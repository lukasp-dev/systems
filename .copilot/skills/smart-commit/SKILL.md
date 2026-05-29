---
name: smart-commit
description: >-
  Stages the right paths, writes an accurate commit message from diffs, and runs
  git commit so the user does not commit manually. Loads the type_correction
  skill for a typo-only pass on prose in scope. Use when the user says smart
  commit, auto commit, commit this, or wants Copilot to finish the commit.
---

# Smart commit

Goal: **Copilot performs the commit** (`git add` + `git commit`). The user should not have to copy a message or run commit themselves unless the flow is blocked (e.g. merge conflict, hook failure).

## Workflow

1. **Inspect**: `git status`, then `git diff` for unstaged tracked changes and `git diff --staged` if anything is already staged. Respect a **narrow scope** if the user named files, directories, or "only X".
2. **Type correction (required)**: Read and follow **[`type_correction/SKILL.md`](../type_correction/SKILL.md)** (or `@type_correction`). For every file in this commit scope that contains narrative prose (e.g. `.md`, long comments in code), apply **only** typographical or spelling fixes allowed there - **do not** change substance, explanations, or the author's style. Skip pure code-only files unless they have obvious comment typos in scope. Re-run `git diff` after edits so the commit message matches final text.
3. **Decide commit boundaries**: If unrelated changes are mixed and the user did not insist on one commit, do **multiple** commits (stage subset A -> commit -> stage subset B -> commit). Otherwise one commit is fine.
4. **Stage**: By default, stage everything with `git add .`. If the user explicitly asks for a narrow scope (specific files/dirs/"only X"), stage only that scope. Skip obvious noise unless requested: editor swap files, `.swp`, build binaries, `.cph/`, accidental huge untracked trees - **ask** or exclude them.
5. **Commit**: Run `git commit` with a good message (use `git commit -m "subject" -m "body"` for a short body, or `-F` with a temp file for longer bodies). Request **git_write** (and network only if hooks need it).
6. **Confirm**: Show `git log -1 --oneline` (and status) so the user sees the result.

## Message style

- **Subject**: imperative mood, ~50 characters when reasonable, no trailing period, no vague filler ("update", "changes").
- **Body** (optional): blank line after subject; 1-4 lines for *why* or non-obvious *what*, not a duplicate file list.
- Prefer **repo conventions** (Conventional Commits, `CONTRIBUTING.md`, commit templates) over generic style.

## Safety

- Do not invent ticket IDs, versions, or `Co-authored-by` unless the user provided them.
- If diffs contain **secrets or credentials**, do not commit; warn and stop.
- If **nothing** should be committed after staging rules, explain why instead of an empty commit.
