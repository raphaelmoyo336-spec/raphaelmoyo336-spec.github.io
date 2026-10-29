#!/usr/bin/env python3
"""Compare benchmark JSON reports without silently changing a saved baseline."""

import argparse
import json
import math
from pathlib import Path
import sys


ENVIRONMENT_FIELDS = (
    "qt_version",
    "os",
    "cpu",
    "system_memory_kib",
    "build_type",
    "compiler",
)


def workload_key(benchmark):
    return json.dumps(benchmark["workload"], sort_keys=True, separators=(",", ":"))


def read_report(path):
    with path.open(encoding="utf-8") as report_file:
        report = json.load(report_file)
    if report.get("schema_version") != 1 or not isinstance(report.get("benchmarks"), list):
        raise ValueError(f"{path} is not a supported benchmark report (schema version 1 required)")
    return report


def update_baseline(current_path, baseline_path):
    report = read_report(current_path)
    baseline_path.parent.mkdir(parents=True, exist_ok=True)
    baseline_path.write_text(
        json.dumps(report, indent=2) + "\n",
        encoding="utf-8",
    )
    print(f"Baseline deliberately updated: {baseline_path}")


def compare(current, baseline, threshold):
    current_environment = current["environment"]
    baseline_environment = baseline["environment"]
    mismatches = [
        field for field in ENVIRONMENT_FIELDS
        if current_environment.get(field) != baseline_environment.get(field)
    ]
    if mismatches:
        raise ValueError(
            "Benchmark environments differ for: " + ", ".join(mismatches)
            + ". Capture and compare on the same machine/build configuration."
        )

    baseline_by_key = {
        (item["name"], workload_key(item)): item for item in baseline["benchmarks"]
    }
    review_count = 0
    for item in current["benchmarks"]:
        key = (item["name"], workload_key(item))
        previous = baseline_by_key.get(key)
        if previous is None:
            print(f"{item['name']}: NO BASELINE for workload {item['workload']}")
            continue
        base_median = previous["median_ms"]
        current_median = item["median_ms"]
        if base_median <= 0 or current_median < 0:
            print(f"{item['name']}: INCOMPARABLE invalid median")
            continue

        change = (current_median - base_median) / base_median * 100
        base_count = max(previous["iterations"], 1)
        current_count = max(item["iterations"], 1)
        base_se = previous["standard_deviation_ms"] / math.sqrt(base_count)
        current_se = item["standard_deviation_ms"] / math.sqrt(current_count)
        uncertainty = 2 * math.hypot(base_se, current_se) / base_median * 100

        if change > threshold + uncertainty:
            status = "REVIEW"
            review_count += 1
        elif change > threshold:
            status = "NOISE"
        elif change < -threshold:
            status = "IMPROVEMENT"
        else:
            status = "PASS"
        print(
            f"{item['name']}: baseline {base_median:.3f} ms, "
            f"current {current_median:.3f} ms, change {change:+.1f}%, "
            f"status {status} (noise margin {uncertainty:.1f}%)"
        )
    return review_count


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--current", type=Path, required=True, help="Current benchmark JSON report")
    parser.add_argument("--baseline", type=Path, required=True, help="Baseline JSON file")
    parser.add_argument("--threshold-percent", type=float, default=10.0)
    parser.add_argument("--update-baseline", action="store_true",
                        help="Explicitly replace the baseline with the current report")
    parser.add_argument("--fail-on-regression", action="store_true",
                        help="Return a failing exit code if a benchmark status is REVIEW")
    args = parser.parse_args()

    try:
        if args.update_baseline:
            update_baseline(args.current, args.baseline)
            return 0
        if not args.baseline.is_file():
            print(f"No baseline found at {args.baseline}; comparison is advisory only.")
            return 0
        reviews = compare(read_report(args.current), read_report(args.baseline),
                          args.threshold_percent)
        return 1 if reviews and args.fail_on_regression else 0
    except (OSError, ValueError, KeyError, TypeError, json.JSONDecodeError) as error:
        print(f"Benchmark comparison failed: {error}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
