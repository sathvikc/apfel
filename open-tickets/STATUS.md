# apfel — Status Overview

**Version:** 0.4.0
**Date:** 2026-03-26
**Build:** ✅ Clean (Swift 6.3 / macOS 26.4 SDK)
**Tests:** ✅ 28/28 passing

---

## Completed (Merged to main)

### v0.3.0 → v0.4.0 (Golden Goals)
- ✅ **Critical bug fix:** History replay no longer re-infers (Transcript API)
- ✅ **Real token counting:** `SystemLanguageModel.tokenCount(for:)` (macOS 26.4)
- ✅ **Context window protection:** Budget-aware history truncation (server + CLI)
- ✅ **Typed error handling:** `ApfelError.classify()` with `GenerationError` matching
- ✅ **Native tool definitions:** `Transcript.ToolDefinition` via `DynamicGenerationSchema`
- ✅ **Native tool call history:** `Transcript.ToolCalls` entries (not serialized JSON)
- ✅ **Streaming tool call detection:** `finish_reason: "tool_calls"` in SSE
- ✅ **`response_format: "json_object"`** support
- ✅ **AsyncSemaphore:** Rewritten, no more double-resume crash
- ✅ **CORS:** Headers on all responses (errors, streaming, preflight OPTIONS)
- ✅ **Server stubs:** `/v1/completions` → 501, `/v1/embeddings` → 501
- ✅ **CLI flags:** `--temperature`, `--seed`, `--max-tokens`, `--permissive`, `--model-info`
- ✅ **`--chat` context rotation:** Auto-truncates when approaching limit
- ✅ **JSON escaping:** `buildSystemPrompt` uses `JSONSerialization`
- ✅ **AnyCodable null:** Proper `decodeNil()` handling
- ✅ **GUI:** Removed fake token counts, works as HTTP consumer

## Open Tickets

| # | Title | Priority | Status |
|---|-------|----------|--------|
| 005 | Python OpenAI client E2E integration tests | P1 | Ready to implement |
| 006 | Opportunistic context summarization | P2 | Design ready |
| 007 | GUI token budget display + streaming usage | P2 | Needs server change |
| 008 | `finish_reason: "length"` detection | P2 | Design ready |
| 009 | Environment variable support | P3 | Simple |

## Architecture

```
┌─────────────────────────────────────────────────┐
│  apfel v0.4.0 — 100% on-device                 │
├─────────────┬───────────────┬───────────────────┤
│  CLI        │  HTTP Server  │  GUI (SwiftUI)    │
│  --chat     │  OpenAI API   │  via APIClient    │
│  --stream   │  /v1/chat/*   │  → localhost HTTP │
│  single     │  /v1/models   │                   │
├─────────────┴───────────────┴───────────────────┤
│  ContextManager (Transcript API)                │
│  SchemaConverter (DynamicGenerationSchema)       │
│  TokenCounter (real tokenCount, macOS 26.4)     │
├─────────────────────────────────────────────────┤
│  FoundationModels.SystemLanguageModel           │
│  (on-device, no network, no cloud)              │
└─────────────────────────────────────────────────┘
```
