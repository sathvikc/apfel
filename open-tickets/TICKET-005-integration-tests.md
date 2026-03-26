# TICKET-005: Integration Tests — Python OpenAI Client E2E

**Status:** Open — READY TO IMPLEMENT
**Priority:** P1 (validation of OpenAI compatibility goal)
**Blocked by:** Nothing (TICKET-003 and TICKET-004 are resolved)

---

## Goal

A Python test suite using the `openai` library that validates all major features
of the apfel OpenAI-compatible server against the actual running binary.

## Updates (v0.4.0)

- Model name is `apple-foundationmodel` (not `apple/on-device-3b`)
- Server now supports `response_format`, native tool definitions, CORS OPTIONS
- Stub endpoints return 501 (not 404)
- Token counts are real (not chars/4)

## Test File Location

`Tests/integration/openai_client_test.py`

## Tests to Implement

```python
import openai
import pytest

client = openai.OpenAI(base_url="http://localhost:11434/v1", api_key="ignored")

def test_basic_completion():
    resp = client.chat.completions.create(
        model="apple-foundationmodel",
        messages=[{"role": "user", "content": "What is 2+2? Reply with just the number."}]
    )
    assert resp.choices[0].message.content is not None
    assert resp.usage.total_tokens > 0

def test_streaming():
    stream = client.chat.completions.create(
        model="apple-foundationmodel",
        messages=[{"role": "user", "content": "Say hello"}],
        stream=True
    )
    content = "".join(chunk.choices[0].delta.content or "" for chunk in stream)
    assert len(content) > 0

def test_multi_turn_history():
    messages = [
        {"role": "user", "content": "My name is TestUser."},
        {"role": "assistant", "content": "Hello TestUser!"},
        {"role": "user", "content": "What is my name?"}
    ]
    resp = client.chat.completions.create(model="apple-foundationmodel", messages=messages)
    assert "TestUser" in resp.choices[0].message.content

def test_temperature_zero():
    kwargs = dict(model="apple-foundationmodel",
                  messages=[{"role": "user", "content": "What is 2+2?"}],
                  temperature=0, seed=42)
    r1 = client.chat.completions.create(**kwargs)
    r2 = client.chat.completions.create(**kwargs)
    assert r1.choices[0].message.content == r2.choices[0].message.content

def test_tool_calling():
    tools = [{
        "type": "function",
        "function": {
            "name": "get_weather",
            "description": "Get weather for a city",
            "parameters": {
                "type": "object",
                "properties": {"city": {"type": "string"}},
                "required": ["city"]
            }
        }
    }]
    resp = client.chat.completions.create(
        model="apple-foundationmodel",
        messages=[{"role": "user", "content": "What's the weather in Vienna?"}],
        tools=tools
    )
    assert resp.choices[0].finish_reason == "tool_calls"
    assert resp.choices[0].message.tool_calls[0].function.name == "get_weather"

def test_tool_calling_streaming():
    tools = [{
        "type": "function",
        "function": {
            "name": "get_weather",
            "description": "Get weather for a city",
            "parameters": {
                "type": "object",
                "properties": {"city": {"type": "string"}},
                "required": ["city"]
            }
        }
    }]
    stream = client.chat.completions.create(
        model="apple-foundationmodel",
        messages=[{"role": "user", "content": "What's the weather in Vienna?"}],
        tools=tools,
        stream=True
    )
    chunks = list(stream)
    last_chunk = chunks[-2]  # before [DONE]
    assert last_chunk.choices[0].finish_reason == "tool_calls"

def test_json_mode():
    resp = client.chat.completions.create(
        model="apple-foundationmodel",
        messages=[{"role": "user", "content": "Return a JSON object with key 'greeting' and value 'hello'"}],
        response_format={"type": "json_object"}
    )
    import json
    parsed = json.loads(resp.choices[0].message.content)
    assert "greeting" in parsed

def test_models_endpoint():
    models = client.models.list()
    assert len(models.data) > 0
    assert models.data[0].id == "apple-foundationmodel"

def test_image_rejection():
    with pytest.raises(openai.BadRequestError) as exc:
        client.chat.completions.create(
            model="apple-foundationmodel",
            messages=[{"role": "user", "content": [
                {"type": "text", "text": "What's in this image?"},
                {"type": "image_url", "image_url": {"url": "http://example.com/img.jpg"}}
            ]}]
        )
    assert "image" in str(exc.value).lower()

def test_completions_stub():
    """Legacy /v1/completions should return 501"""
    import httpx
    resp = httpx.post("http://localhost:11434/v1/completions",
                      json={"model": "apple-foundationmodel", "prompt": "hi"})
    assert resp.status_code == 501

def test_embeddings_stub():
    """Legacy /v1/embeddings should return 501"""
    import httpx
    resp = httpx.post("http://localhost:11434/v1/embeddings",
                      json={"model": "apple-foundationmodel", "input": "hi"})
    assert resp.status_code == 501

def test_cors_preflight():
    import httpx
    resp = httpx.options("http://localhost:11434/v1/chat/completions")
    assert resp.status_code == 204
    assert "access-control-allow-origin" in resp.headers
```

## Running

```bash
pip install openai pytest httpx
swift build -c release
.build/release/apfel --serve --port 11434 &
SERVER_PID=$!
sleep 3  # wait for server startup
python3 -m pytest Tests/integration/openai_client_test.py -v
kill $SERVER_PID
```
