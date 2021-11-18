# Notes and Questions

## Notes: static predictor

* `bunzip2 -kc $filename | ./predictor --custom`
* Formula to compute static predictor size:
  * `0 bits`
  * There is nothing stored in this predictor.

## Notes: gshare predictor

* `bunzip2 -kc $filename | ./predictor --gshare:13`
* Formula to compute gshare predictor size:
  * `(2 * 2^ghistoryBits) + ghistoryBits = (2 bit saturating counter * 2^ghistoryBits indicies in the PHT) + ghistoryBits to store GHR.`
  * `(2 * (2^13)) + 13 = (2 * 8192) + 13 = 16384 + 13 = 16 kilobits + 13 bits`

## Notes: tournament predictor

* `bunzip2 -kc $filename | ./predictor --tournament:9:10:10`
* Formula to compute tournament predictor size:
  * Global Predictor Cost = `(2 * 2^ghistoryBits) + ghistoryBits = (2 bit saturating counter * 2^ghistoryBits indicies in the Global Table) + ghistoryBits to store GHR`
  * Choice Predictor Cost = `(2 * 2^ghistoryBits) + ghistoryBits = (2 bit saturating counter * 2^ghistoryBits indicies in the Choice Table) + ghistoryBits to store GHR`
  * Local Predictor Cost = `((2^pcIndexBits) * lhistoryBits) + (2^lhistoryBits * 2) = (2^pcIndexBits indicies in Local History Table * lhistoryBits used at each index) + 2^lhistoryBits indicies in the Local Table * 2 bit saturating counter)`
  * Total Cost = Choice Predictor Cost + Global Predictor Cost + Local Predictor Cost - ghistoryBits
    * `((2 * 2^ghistoryBits) + ghistoryBits) + ((2 * 2^ghistoryBits) + ghistoryBits) + (((2^pcIndexBits) * lhistoryBits) + (2^lhistoryBits * 2)) - ghistoryBits`
    * `((2 * 2^9) + 9) + ((2 * 2^9) + 9) + (((2^10) * 10) + (2^10 * 2)) - 9`
    * `((2 * 512) + 9) + ((2 * 512) + 9) + ((1024 * 10) + (1024 * 2)) - 9`
    * `(1024 + 9) + (1024 + 9) + (10240 + 2048) - 9`
    * `1024 + 1024 + 10240 + 2048 + 9`
    * `14336 + 9 = 14 kilobits + 9 bits`

## Notes: custom predictor

* `bunzip2 -kc $filename | ./predictor --custom`
* Formula to compute custom predictor size:
  * Global Predictor Cost = `(3 * 2^ghistoryBits) + ghistoryBits = (3 bit saturating counter * 2^ghistoryBits indicies in the Global Table) + ghistoryBits to store GHR`
  * Choice Predictor Cost = `(3 * 2^ghistoryBits) + ghistoryBits = (3 bit saturating counter * 2^ghistoryBits indicies in the Choice Table) + ghistoryBits to store GHR`
  * Local Predictor Cost = `((2^pcIndexBits) * lhistoryBits) + (2^lhistoryBits * 3) = (2^pcIndexBits indicies in Local History Table * lhistoryBits used at each index) + 2^lhistoryBits indicies in the Local Table * 3 bit saturating counter)`
  * Total Cost = Choice Predictor Cost + Global Predictor Cost + Local Predictor Cost - ghistoryBits
    * `((3 * 2^ghistoryBits) + ghistoryBits) + ((3 * 2^ghistoryBits) + ghistoryBits) + (((2^pcIndexBits) * lhistoryBits) + (2^lhistoryBits * 3)) - ghistoryBits`
    * `((3 * 2^9) + 9) + ((3 * 2^9) + 9) + (((2^10) * 10) + (2^10 * 3)) - 9`
    * `((3 * 512) + 9) + ((3 * 512) + 9) + ((1024 * 10) + (1024 * 3)) - 9`