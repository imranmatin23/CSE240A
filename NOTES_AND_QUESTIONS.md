# Notes and Questions

## Notes: gshare predictor

* `bunzip2 -kc $filename | ./predictor --gshare:13`
* 16KB
* Formula to compute gshare size:
  * `ghistoryBits + (PHT_SIZE * n-bit counter size)`

### Pattern History Table

* What size PHT should I use for gshare?
  * Currently using 1024.
* To index into the PHT I perform ((ghr ^ pc) % PHT_SIZE), is that correct?

## Notes: tournament predictor

* `bunzip2 -kc $filename | ./predictor --tournament:9:10:10`
* 14KB
* Formula to compute tournament size:
  * `ghistoryBits + (CHOICE_PREDICT_SIZE * n-bit counter size) + (GLOBAL_PREDICT_SIZE * n-bit counter size) + (LOCAL_PREDICT_SIZE * n-bit counter size) + (LOCAL_HISTORY_TABLE_SIZE * pcIndexBits)`

### Choice Predict Table

* What size Choice Predict Table should I use for gshare?
  * Currently using 4096.
* To index into the Choice Predict Table I perform (ghr % CHOICE_PREDICT_SIZE), is that correct?

### Global Predict Table

* What size Global Predict Table should I use for gshare?
  * Currently using 4096.
* To index into the Global Predict Table I perform (ghr % GLOBAL_PREDICT_SIZE), is that correct?

### Local History Table

* What size Local History Table should I use for tournament?
  * Currently using 1024.
* To index into the Local History Table I use (pc & ((2^pcIndexBits)-1)) % LOCAL_HISTORY_TABLE_SIZE, is that correct?

### Local Predict Table

* What size Predict Table should I use for tournament?
  * Currently using 1024.
* To index into the Predict Table I use (local_history_pattern & ((2^lhistoryBits)-1)) % LOCAL_PREDICT_SIZE?
