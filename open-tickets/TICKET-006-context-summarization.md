# TICKET-006: Opportunistic Context Summarization

**Status:** Open
**Priority:** P2 (quality improvement for long conversations)
**Blocked by:** Nothing

---

## Goal

When the context window approaches 70% capacity in `--chat` mode, use a separate
session to summarize the conversation so far, then start a fresh session with the
summary as the system prompt. This preserves context better than the current
"drop oldest messages" approach.

## Current Behavior

`CLI.swift:truncateTranscript()` drops oldest messages when over budget. This works
but loses important context from earlier in the conversation.

## Proposed Behavior

1. At 70% capacity, spawn a one-shot summarization session:
   ```
   "Summarize this conversation concisely, preserving key facts and decisions:
   [transcript text]"
   ```
2. Use the summary as the new system prompt for a fresh session
3. Keep the last 1-2 turns as explicit history
4. Show `[context summarized]` indicator in CLI

## Notes

- Apple's WWDC25 "Deep Dive" session recommends this pattern
- Must handle the case where summarization itself triggers a guardrail
- Summary session should use `.permissiveContentTransformations` guardrails
