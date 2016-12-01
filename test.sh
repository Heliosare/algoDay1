#!/bin/bash

OUT=$(echo "$3" | $1 "$2")

if echo "$OUT" | awk '
  function abs(x) { if (x < 0) return -x; else return x; }
  BEGIN {
    n[1]="Num nodes"
    n[2]="Num arcs"
    n[3]="Sum of arc durations (seconds)";
    n[4]="Answer to the test query";
    error = 0
  }
  ARGIND == 1 && FNR <= 4 { e[FNR] = $1 }
  ARGIND == 1 && FNR >= 4 { ep[FNR] = $0; }
  ARGIND == 2 && FNR <= 4 {
    num++;
    printf("Testing %s ... ", n[FNR]);
    if (abs($1-e[FNR]) > 0.01) {
      print "ERROR: expected " e[FNR], " got: " $1;
      error++;
    } else print "OK!";
  }
  ARGIND == 2 && FNR >= 4 {
    if (error > 0) next;
    path_size = split(ep[FNR], path)
    if (NF != path_size) {
      print "NO BONUS: Path description length differs (expected " path_size - 1 ", got " NF - 1 ").";
    } else {
      bad = 0
      for (i = 2; i <= path_size; ++i) if (abs($i - path[i]) > 0.01) bad += 1;
      if (bad == 0) {
        print "BONUS+++ fully correct path";
        next;
      }
      if (bad <= 3) {
        print "BONUS++ almost correct path";
        next;
      }
      bad = 0
      for (i = 2; i <= path_size; ++i) if (abs($i - path[path_size + 1 - i]) > 0.01) bad += 1;
      if (bad == 0) {
        print "BONUS++ correct path if reversed";
        next;
      }
      if (bad <= 3) {
        print "BONUS+ almost correct path if reversed";
        next;
      }
      print "BONUS-- Correct path arc count";
    }
  }
  END {
    if (num != 4) {
      error++;
      print "ERROR: Missing or unexpected output (expected 4 lines, got " num ").";
    }
    if (error > 0) exit 1;
  }' "$4" -; then
  echo "PASS"
else
  echo "FAILED"
fi
