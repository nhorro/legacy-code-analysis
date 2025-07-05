#!/usr/bin/env python3
import csv
import subprocess
import sys
import os

def demangle(name):
    """Demangle usando c++filt"""
    if name in ["??", "", None]: return name
    try:
        out = subprocess.check_output(
            ["c++filt"], input=name.encode(), stderr=subprocess.DEVNULL
        )
        return out.decode().strip()
    except Exception:
        return name

def resolve_addr(addr, binary):
    """addr2line + c++filt"""
    addr = addr.strip()
    if not addr or addr == "0x0":
        return ("?", "?")
    try:
        out = subprocess.check_output(
            ["addr2line", "-f", "-e", binary, addr],
            stderr=subprocess.DEVNULL
        ).decode().splitlines()
        if len(out) >= 2:
            mangled = out[0].strip()
            func = demangle(mangled)
            fileline = out[1].strip()
            return (func, fileline)
    except Exception:
        pass
    return ("?", "?")

def main():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <binary> <trace.csv> [output.csv]")
        sys.exit(1)

    binary = sys.argv[1]
    trace_file = sys.argv[2]
    output_file = sys.argv[3] if len(sys.argv) > 3 else "trace_resolved.csv"

    if not os.path.isfile(binary):
        print(f"Error: binary {binary} does not exist.")
        sys.exit(1)

    if not os.path.isfile(trace_file):
        print(f"Error: trace file {trace_file} does not exist.")
        sys.exit(1)

    addr_cache = {}

    with open(trace_file) as f, open(output_file, "w", newline="") as fout:
        reader = csv.reader(f)
        writer = csv.writer(fout)
        # Header
        writer.writerow([
            "timestamp_ns","thread_id","event",
            "callee_addr","callee_func","callee_fileline",
            "caller_addr","caller_func","caller_fileline"
        ])

        for row in reader:
            if row[0].startswith("timestamp"):
                continue  # skip header

            ts, tid, event, callee, caller = row

            if callee not in addr_cache:
                addr_cache[callee] = resolve_addr(callee, binary)
            callee_func, callee_fileline = addr_cache[callee]

            if caller not in addr_cache:
                addr_cache[caller] = resolve_addr(caller, binary)
            caller_func, caller_fileline = addr_cache[caller]

            writer.writerow([
                ts, tid, event,
                callee, callee_func, callee_fileline,
                caller, caller_func, caller_fileline
            ])

    print(f"[OK] Resolved trace written to: {output_file}")

if __name__ == "__main__":
    main()
