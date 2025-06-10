import json
import subprocess
import sys
from pathlib import Path
import os

VM_BINARY = "./bin/nova16"

env = os.environ.copy()
env["PYTHONPATH"] = str(Path("assembler/src").resolve())


def needs_rebuild(vm_binary, source_dirs=["src", "include"]):
    if not os.path.exists(vm_binary):
        return True

    binary_mtime = os.path.getmtime(vm_binary)

    for dir in source_dirs:
        for path in Path(dir).rglob("*.[ch]"):
            if path.stat().st_mtime > binary_mtime:
                return True
    return False


def build_vm_if_needed():
    if needs_rebuild(VM_BINARY):
        print("⚙️  Rebuilding nova16...")
        result = subprocess.run(["task", "build", f"BINARY={VM_BINARY}"])
        if result.returncode != 0:
            print("❌ Build failed.")
            sys.exit(1)


def compile_nova_to_bin(source_path: Path) -> Path:
    bin_path = Path("bin") / (source_path.stem + ".bin")
    result = subprocess.run(
        [
            "python3",
            "assembler/src/asm/__init__.py",
            str(source_path),
            "-o",
            str(bin_path),
        ],
        capture_output=True,
        text=True,
        env=env,
    )
    if result.returncode != 0:
        print(f"❌ Assembly failed for {source_path}")
        print(result.stderr.strip())
        sys.exit(1)
    return bin_path


def run_test(test_file):
    test_name = test_file.stem
    source_path = test_file.with_suffix(".nova")
    if not source_path.exists():
        print(f"❌ Missing source file: {source_path}")
        return False

    bin_path = compile_nova_to_bin(source_path)

    with open(test_file, "r") as f:
        test = json.load(f)

    expected = test.get("expect", {})
    cmd = [VM_BINARY, str(bin_path), "--trace-json"]
    result = subprocess.run(cmd, capture_output=True, text=True)

    try:
        output = json.loads(result.stderr)
    except json.JSONDecodeError as e:
        print(f"❌ Failed to parse JSON output for {test_file}")
        print("📎 JSON error:", e)
        print("📤 Raw output:")
        print("--------------------")
        print(result.stdout.strip() or "(empty output)")
        print("--------------------")
        print("💡 Hints:")
        print(" - Did you forget to call json_dump_state() in your VM?")
        print(" - Did your program crash before HLT?")
        print(" - Did you pass '--trace-json'?")
        print(" - Command that was run: \033[1m" + " ".join(cmd) + "\033[0m")
        print()
        return False

    failed = False

    if "registers" in expected:
        for reg, val in expected["registers"].items():
            if output["registers"].get(reg) != val:
                print(f"❌ {reg} = {output['registers'].get(reg)}, expected {val}")
                failed = True

    if "memory" in expected:
        for addr, val in expected["memory"].items():
            actual = output["memory"].get(addr)
            if actual != val:
                print(f"❌ mem[{addr}] = {actual}, expected {val}")
                failed = True

    if not failed:
        print(f"✅ {test_file} passed")
    return not failed


def main():
    print("🧪 Running nova16 test suite...\n")

    test_dir = Path("tests")
    test_files = list(test_dir.rglob("*.json"))

    build_vm_if_needed()

    passed = 0
    for test_file in test_files:
        if run_test(test_file):
            passed += 1

    total = len(test_files)
    print(f"\nPassed {passed} / {total} tests")
    if passed != total:
        sys.exit(1)


if __name__ == "__main__":
    main()
