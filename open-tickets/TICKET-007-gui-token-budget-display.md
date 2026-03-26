# TICKET-007: GUI Token Budget Display

**Status:** Open
**Priority:** P2 (developer experience)
**Blocked by:** Nothing

---

## Goal

Show a real-time token usage bar in the GUI debug panel so developers can see
context window consumption: `current / 4096 tokens`.

## Current Behavior

- Token count per message shows `nil` (was fake chars/4, now removed)
- No visibility into total context consumption
- No warning when approaching the limit

## Proposed Behavior

1. After each response, read `usage` from the server response (non-streaming)
   or estimate from accumulated content (streaming)
2. Show a progress bar in the debug panel: `Tokens: 2847 / 4096 (70%)`
3. Color-code: green (<50%), yellow (50-80%), red (>80%)
4. For streaming, the final SSE chunk could include usage data (requires server change)

## Server Change Needed

Add `usage` to the final streaming chunk (OpenAI's `stream_options: {"include_usage": true}`
pattern). Currently streaming responses don't report usage stats.

## Files

- `Sources/GUI/DebugPanel.swift` — add token budget display
- `Sources/GUI/ChatViewModel.swift` — extract usage from responses
- `Sources/Handlers.swift` — optionally include usage in final stream chunk
- `Sources/GUI/APIClient.swift` — parse usage from SSE
