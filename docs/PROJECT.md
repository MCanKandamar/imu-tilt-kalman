# Project
<!-- agent-lite v5.1. Lasting knowledge: read only the section a task needs. -->

## Overview
- **What:** a two-state Kalman filter (tilt angle, gyro bias) in portable C++. It fuses the LSM6DSL gyroscope and accelerometer into one tilt angle.
- **Why:** a portfolio project that teaches C++, Linux, Docker and CI/CD through a problem close to the owner's control and embedded background.
- **Users:** the owner, and recruiters or interviewers reading the GitHub repository.
- **Test idea:** GitHub's CI servers have no board, so the filter is tested in three stages:
  1. Simulation: generated data with a known true angle, on the PC and in CI.
  2. Replay: real sensor data recorded from the STM32 as CSV files, on the PC and in CI.
  3. Live: the filter runs on the STM32 with live sensor data.
- **Out of scope:** yaw (needs a magnetometer), multi-axis attitude, non-linear filters (EKF), RTOS, Eigen.

## Architecture
Planned layout; update it when each step creates the real files.

| Path | Purpose |
|---|---|
| `core/` | Filter and `Matrix<R, C>`; portable, no HAL, no heap |
| `sim/` | Sensor simulator with a known true angle |
| `tests/` | Unit, simulation and replay tests |
| `data/` | Recorded sensor logs (CSV) |
| `tools/` | Python plotting scripts |
| `firmware/` | CubeMX project: sensor reading, UART logging, filter on target |
| `docker/` | Image with the host and ARM toolchains |
| `.github/workflows/` | CI |

Dependency rule: `firmware/`, `sim/` and `tests/` use `core/`; `core/` depends on none of them.

## Environment
- Main development OS: Ubuntu (dual boot). Windows 11 is used for planning and may be used for STM32CubeIDE.
- Repository path on Windows: `C:\Users\msmca\projects\imu-tilt-kalman`. The path on Ubuntu is an open question in `docs/WORK.md`.
- Sync between the two systems: GitHub. Remote not created yet; it starts private.
- Shared NTFS data disk: `E:`.
- Windows Fast Startup was on when checked on 2026-10-31 (`HiberbootEnabled = 1`). Use Restart, not Shut down, before booting Ubuntu, or Ubuntu may mount NTFS disks read-only or not at all.
- Windows `~/.agents/USER.md` names PowerShell and a Windows signing-key path, so Ubuntu cannot use that file unchanged.

## Hardware & toolchain
- Target: STM32WLE5JCI6 (Cortex-M4, UFBGA73) on a custom board; board model not recorded. No FPU: `__FPU_PRESENT` is 0 in `stm32wle5xx.h:142`.
- Sensor: LSM6DSL on the board, I2C address `0xD6` (8-bit form).
- Toolchain & build: `arm-none-eabi-gcc`, flags `-mcpu=cortex-m4 -mthumb -mfloat-abi=soft`. STM32CubeIDE was used in the earlier project; the CMake build is created in the firmware step.
- Code generator: STM32CubeMX; the new firmware project does not exist yet.
- Pins, clocks, peripherals: earlier project used I2C2 with SDA = PA15, SCL = PB15, and SWD on PA13/PA14. Clock was MSI 4 MHz. No UART was configured. The device header defines USART1, USART2 and LPUART1, and no USB peripheral.
- RTOS & memory budget: no RTOS. Flash 256 KB, RAM 64 KB + RAM2 32 KB (`STM32WLE5JCIX_FLASH.ld:47-49`).
- Debug & flashing setup: ST-Link. Flashing needs the user's approval every time.

## Decisions & constraints
- 2026-10-31 · Decision: use the board and LSM6DSL from the earlier project · Why: the owner already has them and knows the I2C setup.
- 2026-10-31 · Decision: sample at 104 Hz · Why: the earlier 26 Hz is low for tilt estimation, and 104 Hz leaves a 9.6 ms budget per sample.
- 2026-10-31 · Constraint: no hardware FPU · Learned from: `stm32wle5xx.h:142`. Use `float` and measure the filter's cycles with the DWT counter. A rough, unmeasured estimate of 15,000 cycles per step would take about 40% of the budget at 4 MHz and 3% at 48 MHz.
- 2026-10-31 · Decision: write our own `Matrix<R, C>` instead of using Eigen · Why: small matrices only, and it is the main C++ exercise.
- 2026-10-31 · Decision: the repository starts private and goes public at the last step · Why: the owner's choice.
- 2026-10-31 · Decision: license is MIT · Why: common for portfolio libraries; proposed by the agent and not objected to.

## References
- Earlier project: `C:\Users\msmca\Documents\STM32\004_MEMS_I2C` (read-only). `lsm6dsl_mems.c/.h` hold register and sensitivity definitions. Its `main.c:147` has the sensor parse call commented out, so `accelData` and `gyroData` stay 0 in the current version.
