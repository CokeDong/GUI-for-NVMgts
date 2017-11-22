# GUI-for-NVMgts
the GUI of NVMgts

# Test Procedure
## cache factor ##
由于CPU cache的原因，对NVM的模拟延迟的倍数可能并没有设置的那么大，为尽量减少此影响，尽量采用300MB以上数据集，避免cache的影响。在自己的测试中，100M数据集下影响较大。
## Disk Space Warning ##
由于每次运行都包含上千query，这些结果写入到磁盘中会导致大量的磁盘空间消耗，所以要运行几次就删除一次RangeQueryResult.txt，以防止该问题。

# Last Updated #
2017.11.22
