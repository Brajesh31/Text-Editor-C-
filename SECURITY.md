# Security Policy

## Project security status

This repository contains a small educational C terminal editor. It is not a hardened application and must not be treated as a secure editor, a sandbox, or a tool for processing untrusted data.

The current implementation uses fixed-size global arrays and several string operations that do not consistently prove that destination buffers are large enough. The input loop also does not handle end-of-file correctly. These are meaningful correctness and safety concerns, not merely style issues.

## Supported versions

Security fixes should target the latest commit on the default branch. There are no separately maintained release branches or guaranteed security support windows.

## Reporting a vulnerability

Please do not publish a working exploit or sensitive details in a public issue.

Use GitHub's private vulnerability reporting feature if it is enabled for this repository. Otherwise, contact the repository owner privately through GitHub. Include:
- A concise description of the issue.
- The affected file and function, if known.
- Compiler and operating system details.
- A minimal input sequence or reproducer.
- The impact you observed.
- A suggested mitigation, if you have one.

Do not attach credentials, personal information, or unrelated private data.

## Response expectations

The maintainer will review a report as promptly as practical. There is no guaranteed response or remediation time. Please allow time for reproduction and validation before publishing details.

If a report is accepted, a fix should include a regression test or a documented manual reproduction where practical. The project should not claim a vulnerability is fixed until the relevant change has been reviewed and validated.

## Known risk areas

The following areas deserve particular care:

| Area | Current concern | Safer direction |
|---|---|---|
| Line input | Fixed-size input and unchecked `fgets` result | Check EOF and explicitly detect or reject truncated input |
| Line storage | Fixed 256-byte line slots | Enforce a documented maximum and guarantee null termination |
| Replacement | Uses concatenation into a fixed local array | Calculate output length before writing, then use a bounded strategy |
| Keyword display | Copies input into a fixed local array | Bound the copy and preserve the original line |
| Command parsing | Uses `sscanf` with fixed arrays | Parse with explicit length checks and clear error handling |
| Buffer indexes | User-provided indexes select fixed arrays | Validate before every access |
| History | Fixed arrays of complete state snapshots | Define overflow behavior and keep history consistent across buffers |
| Terminal clearing | Invokes a shell command through `system` | Prefer terminal escape sequences or a platform abstraction |

## Safe-use guidance

- Do not run the program with untrusted or adversarial input.
- Do not assume that malformed input will be rejected safely.
- Do not use the program to edit important files, because it has no persistence and may lose all text at exit.
- Compile with warnings and, where available, AddressSanitizer and UndefinedBehaviorSanitizer.
- Keep generated executables out of commits unless a deliberate release process is introduced.

## Scope

This policy covers security issues in this repository's source code, build instructions, workflows, and project-maintained documentation. It does not provide security guarantees for third-party compilers, terminals, operating systems, or external tools.
