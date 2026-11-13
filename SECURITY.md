# Security Policy

This project is an educational C program and is not hardened for untrusted input.

## Reporting

Do not publish exploit details that could put users at risk. Use GitHub private vulnerability reporting if enabled, or contact the repository owner privately through GitHub.

## Current risk areas

The implementation uses fixed-size buffers and string operations that require additional bounds checking. Do not run it with untrusted input or treat it as a secure editor. Security-related fixes should include a minimal reproducer and a regression test where possible.
