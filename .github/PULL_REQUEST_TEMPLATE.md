# Pull Request

## Summary
Describe the problem and the exact change. Link the related issue where applicable.

## Change category
- [ ] Bug fix
- [ ] Feature
- [ ] Refactoring
- [ ] Documentation
- [ ] Tests or CI
- [ ] Build tooling

## Implementation details
Explain the important implementation decisions. For C changes, describe relevant bounds checks, input validation, memory ownership, and error handling.

## Behavior and compatibility
- [ ] Existing command behavior is preserved, or changes are documented.
- [ ] Buffer and line limits are handled explicitly.
- [ ] Undo/redo behavior is consistent for every changed mutation.
- [ ] Buffer switching does not introduce unintended state leakage.
- [ ] Documentation reflects the actual implementation.

## Validation
- [ ] Compiled with GCC using C11 and warnings enabled.
- [ ] Tested affected commands manually.
- [ ] Tested invalid and boundary inputs.
- [ ] Ran automated tests, if available.
- [ ] Ran sanitizers, if relevant and available.
- [ ] No generated binaries or local environment files were added.

## Test evidence
Paste commands and concise results. If a check was not run, say so.

## Risk and follow-up
Describe known limitations, migration concerns, and follow-up work. Do not mark planned behavior as implemented.
