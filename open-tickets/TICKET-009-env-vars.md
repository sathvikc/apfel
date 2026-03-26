# TICKET-009: Environment Variable Support

**Status:** Open
**Priority:** P3 (convenience)
**Blocked by:** Nothing

---

## Goal

Support configuration via environment variables for scriptable usage.

## Variables

- `APFEL_SYSTEM_PROMPT` — default system prompt (overridden by `--system`)
- `APFEL_HOST` — server bind address (overridden by `--host`)
- `APFEL_PORT` — server port (overridden by `--port`)
- `APFEL_TEMPERATURE` — default temperature
- `APFEL_MAX_TOKENS` — default max tokens

## Files

- `Sources/main.swift` — read env vars as defaults, CLI flags override
