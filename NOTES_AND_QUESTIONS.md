# Notes and Questions

## Notes: gshare predictor

* `bunzip2 -kc $filename | ./predictor --gshare:13`
* Formula to compute gshare size:
  * `2 * 2^ghistoryBits`
  * `2 * (2^13) = 2 * 8192 = 16384 = 16 kilobits`
* Number of indices in pattern history table = (2^ghistoryBits)

## Notes: tournament predictor

* `bunzip2 -kc $filename | ./predictor --tournament:9:10:10`
* Formula to compute tournament size:
  * `((2^pcIndexBits) * lhistoryBits) + ((2^lhistoryBits) * 2) + ((2^ghistoryBits) * 2) + ((2^ghistoryBits) * 2)`
  * `((2^10) * 10) + ((2^10) * 2) + ((2^9) * 2) + ((2^9) * 2) = 10240 + 2048 + 1024 + 1024 = 14336 = 14 kilobits`
* Number of indices in local history table = (2^pcIndexBits)
* Number of indices in local predict table = (2^lhistoryBits)
* Number of indices in global predict table = (2^ghistoryBits)
* Number of indices in choice predict table = (2^ghistoryBits)
