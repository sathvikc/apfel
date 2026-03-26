# TICKET-008: Proper finish_reason: "length" Detection

**Status:** Open
**Priority:** P2 (API compatibility)
**Blocked by:** Nothing

---

## Goal

When a response is truncated because it hit `max_tokens`, return
`finish_reason: "length"` instead of `"stop"`.

## Current Behavior

Always returns `"stop"` or `"tool_calls"`. No detection of truncation.

## Challenge

The FoundationModels `Response` struct has no `finishReason` or `isTruncated`
property. We need to infer truncation by comparing response length against
the `max_tokens` parameter, or by checking if the response ends mid-sentence.

## Proposed Approach

If `max_tokens` was set in the request AND the response token count equals
`max_tokens`, return `finish_reason: "length"`.
