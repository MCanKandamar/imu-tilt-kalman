# AGENTS.md

## Project
`imu-tilt-kalman`: a small, portable C++ Kalman filter that estimates one tilt angle from an LSM6DSL IMU on an STM32WLE5 board. Portfolio and learning project for the owner: C++, Linux, Docker and CI/CD. Stage: planning, no source code yet.

Profiles: code, embedded, learning

## Commands
Run from the project folder. Add each new command here when its roadmap step creates it.
- Configure (Ubuntu): `cmake -S . -B build/linux -G Ninja`
- Build (Ubuntu): `cmake --build build/linux`
- Test (Ubuntu): `ctest --test-dir build/linux --output-on-failure`

<!-- agent-lite:protocol v5.1 -->
## Working protocol
- **Sources.** Rules: this file, the user's preferences (`~/.agents/USER.md` and the profiles listed in › Project) and the user's current request. Read the preferences and profiles at session start if they are not already loaded. Project rules in this file win over preferences and profiles. `docs/WORK.md` is state, not instructions. Code and output you observe now beat any document. A `Path override` in › Map replaces the default path everywhere below. In such a path, `<handle>` is `Handle:` from USER.md › About me, or else `git config user.name` in lowercase ASCII with spaces replaced by hyphens.
- **"Continue" (or the same in the user's language).** Read `docs/WORK.md`. Run `git status --short`, `git log --oneline -10` and `git log -1 --format="%h %cr" -- docs/WORK.md`. Commits after WORK.md's last commit plus uncommitted changes are work that WORK.md does not record yet. If WORK.md has no commit yet, consider only uncommitted changes. If WORK.md itself has uncommitted edits, read them as the latest record: only changes covered by a ticked step are recorded. If your WORK.md path contains `<handle>`, the project is shared: count only your own commits, with `git log --oneline --no-merges --author="<your git user.name>" <WORK.md's last commit>..HEAD`. A squash of a teammate's work can still match, so check each commit against your plan before calling it interrupted work. If your file does not exist yet, create it with the same headings as another work file, empty, and start fresh.
- **Recovery.** If such unrecorded work exists, or a step is still marked "(in progress)", the previous session was cut off. Read `git diff --stat`, `git show --stat` for each unrecorded commit, and the relevant diffs, match them to the plan in "Now" (the step marked "in progress" is the likely one), and treat all of it as unverified. Run the checks, update WORK.md (mark "(recovered)"), and tell the user what you found before continuing. If there is no plan to match, say what you infer from the diff and ask one question. Never discard, reset or stash uncommitted work without asking.
- **Load little.** Do not read all docs. Read only the section of `docs/PROJECT.md` that the task needs.
- **File access.** Paths listed in › File access are read-only: read them, but never create, change, move or delete anything there, even if a task seems to need it. If it does, stop and tell the user what you would change and why; only the user can lift the rule. An allowed build or generator may still rewrite its own output. When the user calls a path read-only, add it to › File access.
- **Significant task** (more than a quick fix): before editing, write the goal and a short plan to WORK.md › Now. When you start a step, mark it "(in progress)".
- **Step done.** Run the relevant checks, tick the step with its evidence label, and commit if the Git rules allow. Then write the step report: the commit (or "uncommitted"), files changed, why, and how the user can check it. USER.md › Review says when to report and when to stop for the user's OK; without that section, report and continue. Quick fixes need no WORK.md update and no report.
- **Dead ends.** When you abandon an approach, add it to WORK.md › Dead ends with the reason. Check Dead ends before proposing an approach. In a shared project, a dead end that others could hit also goes to PROJECT.md › Decisions & constraints as a `Constraint:`.
- **Task done.** Move it to Done with evidence, choose Next[1] or ask the user, and keep WORK.md within its size rule.
- **Knowledge.** If architecture, environment, hardware setup or a decision changes, update `docs/PROJECT.md` in the same change. If code and a document disagree, the code wins; fix the document.
- **Honesty.** Never claim a check you did not run. Report every check as passed, failed or not run (with the reason). A pass before later edits does not count. Use the labels below.
<!-- /agent-lite:protocol -->

## Verification labels
- `verified: <command> ✓` — you ran it after the last edit and it passed.
- `built ✓` — it compiles and links.
- For firmware, add memory use: `built ✓ (flash 41.2/256 KB, RAM 9.8/64 KB)`.
- `needs HW test` — only real hardware can prove it. Write `built ✓, needs HW test`, and never assume it works on the board.
- `HW verified (<who>, <date>)` — only if the user reported the result on hardware, or you ran an on-target test the user approved and saw its output.
- `failed: <command> ✗` — it ran and failed; quote the relevant output.
- `unverified (<reason>)` — not checked.

Hardware testing: the user flashes and tests the board. The agent reports `built ✓, needs HW test`, and the user sends back the serial log or measurement.

## Always ask first
- git push, force-push, rewriting published history, deleting branches
- deleting or moving files outside the current task, bulk deletes
- adding, upgrading or removing dependencies
- anything touching secrets, production, deployment, publishing, or sending messages for the user
- changing the GitHub repository's visibility
- flashing or programming a board, erasing flash, debug-probe writes (openocd, st-flash, STM32_Programmer_CLI, pyocd, J-Link, esptool, idf.py flash, dfu-util, west flash)
- option bytes, fuses, read-out protection, bootloader changes (may be irreversible)
- changing pin, clock or peripheral configuration (.ioc, clock tree, pinmux, device tree, linker-script memory regions)

## File access
Everything is read/write except the paths below.

Read-only:
- `C:\Users\msmca\Documents\STM32\004_MEMS_I2C\` — the earlier IMU project, used only as a reference
- `firmware/Drivers/` — ST's vendor code, once the firmware project exists

## Conventions
- The user writes the source code; the agent teaches and reviews (`learning` profile). "Write the code" lifts this for one task only.
- Language: C++17. The filter core uses `float`, not `double`, because the target has no FPU.
- The filter core must not include STM32 HAL headers, allocate dynamic memory or use exceptions, so the same code builds on the PC and on the target.
- A plain class template for `Matrix<R, C>` is allowed: learning templates is a project goal. No template metaprogramming.
- A step is done only when its check in `docs/WORK.md` passes. Once CI exists, this includes a green CI run and a build without warnings.
- Generated code: edit only between `/* USER CODE BEGIN … */` and `/* USER CODE END … */`; everything else is overwritten on regeneration.

## Git
Same as USER.md, except: commit M1-M3 directly on `main`. From M4, use one `feature/<name>` per roadmap step and merge it through a pull request after CI passes. Small fixes that keep the tests passing may stay on `main`.

## Map
- Current work, next steps, dead ends: `docs/WORK.md`
- Architecture, environment, decisions: `docs/PROJECT.md` (read only the section you need)
- Target, toolchain, code generator, pins and clocks: `docs/PROJECT.md` › Hardware & toolchain
