# Table

| Method | First Allocation | First Allocation Time (ms) | Last Alocation | Last Allocation Time | Failed Allocation |
| kmalloc | 4096 | 0 | 4194304 | 1 | 8388608 |
| vmalloc | 4096 | 0 | 2147483648 | 3602 | 4294967296 |
| kmem_cache | 4096 | 0 | 2147483648 | 204 | 4294967296 |
| mempool | 4096 | 0 | 2147483648 | 280 | 4294967296 |
| get_page | 4096 | 0 | 2147483648 | 240 | 4294967296 |


## Data
=========================================================
[  498.028418] ex_kmalloc: kmalloc: 4096 byte
[  498.028420] ex_kmalloc: kmalloc: SUCCSESS
[  498.028421] ex_kmalloc: kmalloc: 4096 byte, 0 ms, type: kmalloc
[  498.028423] ex_kmalloc: kmalloc: 8192 byte
[  498.028425] ex_kmalloc: kmalloc: SUCCSESS
[  498.028426] ex_kmalloc: kmalloc: 8192 byte, 0 ms, type: kmalloc
[  498.028427] ex_kmalloc: kmalloc: 16384 byte
[  498.028430] ex_kmalloc: kmalloc: SUCCSESS
[  498.028431] ex_kmalloc: kmalloc: 16384 byte, 0 ms, type: kmalloc
[  498.028433] ex_kmalloc: kmalloc: 32768 byte
[  498.028436] ex_kmalloc: kmalloc: SUCCSESS
[  498.028437] ex_kmalloc: kmalloc: 32768 byte, 0 ms, type: kmalloc
[  498.028438] ex_kmalloc: kmalloc: 65536 byte
[  498.028443] ex_kmalloc: kmalloc: SUCCSESS
[  498.028444] ex_kmalloc: kmalloc: 65536 byte, 0 ms, type: kmalloc
[  498.028446] ex_kmalloc: kmalloc: 131072 byte
[  498.028522] ex_kmalloc: kmalloc: SUCCSESS
[  498.028523] ex_kmalloc: kmalloc: 131072 byte, 0 ms, type: kmalloc
[  498.028525] ex_kmalloc: kmalloc: 262144 byte
[  498.028543] ex_kmalloc: kmalloc: SUCCSESS
[  498.028543] ex_kmalloc: kmalloc: 262144 byte, 0 ms, type: kmalloc
[  498.028545] ex_kmalloc: kmalloc: 524288 byte
[  498.028577] ex_kmalloc: kmalloc: SUCCSESS
[  498.028578] ex_kmalloc: kmalloc: 524288 byte, 0 ms, type: kmalloc
[  498.028580] ex_kmalloc: kmalloc: 1048576 byte
[  498.028643] ex_kmalloc: kmalloc: SUCCSESS
[  498.028644] ex_kmalloc: kmalloc: 1048576 byte, 0 ms, type: kmalloc
[  498.028646] ex_kmalloc: kmalloc: 2097152 byte
[  498.028776] ex_kmalloc: kmalloc: SUCCSESS
[  498.028777] ex_kmalloc: kmalloc: 2097152 byte, 0 ms, type: kmalloc
[  498.028781] ex_kmalloc: kmalloc: 4194304 byte
[  498.029063] ex_kmalloc: kmalloc: SUCCSESS
[  498.029066] ex_kmalloc: kmalloc: 4194304 byte, 1 ms, type: kmalloc
[  498.029070] ex_kmalloc: kmalloc: 8388608 byte
[  498.029072] ex_kmalloc: kmalloc: FAIL , err_msg = error
==========================================
[  690.961919] ex_vmalloc: vmalloc: 4096 byte
[  690.961922] ex_vmalloc: vmalloc: SUCCSESS
[  690.961923] ex_vmalloc: vmalloc: 4096 byte, 0 ms, type: vmalloc
[  690.961926] ex_vmalloc: vmalloc: 8192 byte
[  690.961927] ex_vmalloc: vmalloc: SUCCSESS
[  690.961928] ex_vmalloc: vmalloc: 8192 byte, 0 ms, type: vmalloc
[  690.961930] ex_vmalloc: vmalloc: 16384 byte
[  690.961932] ex_vmalloc: vmalloc: SUCCSESS
[  690.961933] ex_vmalloc: vmalloc: 16384 byte, 0 ms, type: vmalloc
[  690.961935] ex_vmalloc: vmalloc: 32768 byte
[  690.961939] ex_vmalloc: vmalloc: SUCCSESS
[  690.961939] ex_vmalloc: vmalloc: 32768 byte, 0 ms, type: vmalloc
[  690.961942] ex_vmalloc: vmalloc: 65536 byte
[  690.961946] ex_vmalloc: vmalloc: SUCCSESS
[  690.961946] ex_vmalloc: vmalloc: 65536 byte, 0 ms, type: vmalloc
[  690.961949] ex_vmalloc: vmalloc: 131072 byte
[  690.961956] ex_vmalloc: vmalloc: SUCCSESS
[  690.961957] ex_vmalloc: vmalloc: 131072 byte, 0 ms, type: vmalloc
[  690.961961] ex_vmalloc: vmalloc: 262144 byte
[  690.961975] ex_vmalloc: vmalloc: SUCCSESS
[  690.961976] ex_vmalloc: vmalloc: 262144 byte, 0 ms, type: vmalloc
[  690.961983] ex_vmalloc: vmalloc: 524288 byte
[  690.962009] ex_vmalloc: vmalloc: SUCCSESS
[  690.962010] ex_vmalloc: vmalloc: 524288 byte, 0 ms, type: vmalloc
[  690.962022] ex_vmalloc: vmalloc: 1048576 byte
[  690.962155] ex_vmalloc: vmalloc: SUCCSESS
[  690.962156] ex_vmalloc: vmalloc: 1048576 byte, 0 ms, type: vmalloc
[  690.962179] ex_vmalloc: vmalloc: 2097152 byte
[  690.962297] ex_vmalloc: vmalloc: SUCCSESS
[  690.962298] ex_vmalloc: vmalloc: 2097152 byte, 0 ms, type: vmalloc
[  690.962341] ex_vmalloc: vmalloc: 4194304 byte
[  690.962680] ex_vmalloc: vmalloc: SUCCSESS
[  690.962682] ex_vmalloc: vmalloc: 4194304 byte, 1 ms, type: vmalloc
[  690.962772] ex_vmalloc: vmalloc: 8388608 byte
[  690.963600] ex_vmalloc: vmalloc: SUCCSESS
[  690.963602] ex_vmalloc: vmalloc: 8388608 byte, 1 ms, type: vmalloc
[  690.963779] ex_vmalloc: vmalloc: 16777216 byte
[  690.968773] ex_vmalloc: vmalloc: SUCCSESS
[  690.968777] ex_vmalloc: vmalloc: 16777216 byte, 5 ms, type: vmalloc
[  690.969241] ex_vmalloc: vmalloc: 33554432 byte
[  690.972544] ex_vmalloc: vmalloc: SUCCSESS
[  690.972548] ex_vmalloc: vmalloc: 33554432 byte, 4 ms, type: vmalloc
[  690.973557] ex_vmalloc: vmalloc: 67108864 byte
[  690.979494] ex_vmalloc: vmalloc: SUCCSESS
[  690.979498] ex_vmalloc: vmalloc: 67108864 byte, 6 ms, type: vmalloc
[  690.981564] ex_vmalloc: vmalloc: 134217728 byte
[  691.009167] ex_vmalloc: vmalloc: SUCCSESS
[  691.009171] ex_vmalloc: vmalloc: 134217728 byte, 27 ms, type: vmalloc
[  691.013421] ex_vmalloc: vmalloc: 268435456 byte
[  691.272424] ex_vmalloc: vmalloc: SUCCSESS
[  691.272428] ex_vmalloc: vmalloc: 268435456 byte, 259 ms, type: vmalloc
[  691.282643] ex_vmalloc: vmalloc: 536870912 byte
[  692.014992] ex_vmalloc: vmalloc: SUCCSESS
[  692.014995] ex_vmalloc: vmalloc: 536870912 byte, 732 ms, type: vmalloc
[  692.033931] ex_vmalloc: vmalloc: 1073741824 byte
[  694.098580] ex_vmalloc: vmalloc: SUCCSESS
[  694.098584] ex_vmalloc: vmalloc: 1073741824 byte, 2065 ms, type: vmalloc
[  694.139952] ex_vmalloc: vmalloc: 2147483648 byte
[  697.742091] ex_vmalloc: vmalloc: SUCCSESS
[  697.742094] ex_vmalloc: vmalloc: 2147483648 byte, 3602 ms, type: vmalloc
[  697.817847] ex_vmalloc: vmalloc: 4294967296 byte
============================================================
[ 2506.255238] ex_mempool: mempool: 4096 byte by pages 1
[ 2506.255241] ex_mempool: mempool: SUCCSESS
[ 2506.255242] ex_mempool: mempool: 4096 byte, 0 ms, type: mempool
[ 2506.255244] ex_mempool: mempool: 8192 byte by pages 2
[ 2506.255246] ex_mempool: mempool: SUCCSESS
[ 2506.255247] ex_mempool: mempool: 8192 byte, 0 ms, type: mempool
[ 2506.255248] ex_mempool: mempool: 16384 byte by pages 4
[ 2506.255253] ex_mempool: mempool: SUCCSESS
[ 2506.255254] ex_mempool: mempool: 16384 byte, 0 ms, type: mempool
[ 2506.255255] ex_mempool: mempool: 32768 byte by pages 8
[ 2506.255258] ex_mempool: mempool: SUCCSESS
[ 2506.255259] ex_mempool: mempool: 32768 byte, 0 ms, type: mempool
[ 2506.255260] ex_mempool: mempool: 65536 byte by pages 16
[ 2506.255268] ex_mempool: mempool: SUCCSESS
[ 2506.255269] ex_mempool: mempool: 65536 byte, 0 ms, type: mempool
[ 2506.255270] ex_mempool: mempool: 131072 byte by pages 32
[ 2506.255283] ex_mempool: mempool: SUCCSESS
[ 2506.255283] ex_mempool: mempool: 131072 byte, 0 ms, type: mempool
[ 2506.255285] ex_mempool: mempool: 262144 byte by pages 64
[ 2506.255309] ex_mempool: mempool: SUCCSESS
[ 2506.255310] ex_mempool: mempool: 262144 byte, 0 ms, type: mempool
[ 2506.255311] ex_mempool: mempool: 524288 byte by pages 128
[ 2506.255363] ex_mempool: mempool: SUCCSESS
[ 2506.255363] ex_mempool: mempool: 524288 byte, 0 ms, type: mempool
[ 2506.255365] ex_mempool: mempool: 1048576 byte by pages 256
[ 2506.255483] ex_mempool: mempool: SUCCSESS
[ 2506.255484] ex_mempool: mempool: 1048576 byte, 0 ms, type: mempool
[ 2506.255485] ex_mempool: mempool: 2097152 byte by pages 512
[ 2506.255749] ex_mempool: mempool: SUCCSESS
[ 2506.255750] ex_mempool: mempool: 2097152 byte, 0 ms, type: mempool
[ 2506.255752] ex_mempool: mempool: 4194304 byte by pages 1024
[ 2506.256559] ex_mempool: mempool: SUCCSESS
[ 2506.256561] ex_mempool: mempool: 4194304 byte, 1 ms, type: mempool
[ 2506.256562] ex_mempool: mempool: 8388608 byte by pages 2048
[ 2506.257716] ex_mempool: mempool: SUCCSESS
[ 2506.257718] ex_mempool: mempool: 8388608 byte, 1 ms, type: mempool
[ 2506.257720] ex_mempool: mempool: 16777216 byte by pages 4096
[ 2506.260622] ex_mempool: mempool: SUCCSESS
[ 2506.260626] ex_mempool: mempool: 16777216 byte, 2 ms, type: mempool
[ 2506.260628] ex_mempool: mempool: 33554432 byte by pages 8192
[ 2506.270577] ex_mempool: mempool: SUCCSESS
[ 2506.270581] ex_mempool: mempool: 33554432 byte, 9 ms, type: mempool
[ 2506.270583] ex_mempool: mempool: 67108864 byte by pages 16384
[ 2506.280027] ex_mempool: mempool: SUCCSESS
[ 2506.280031] ex_mempool: mempool: 67108864 byte, 8 ms, type: mempool
[ 2506.280034] ex_mempool: mempool: 134217728 byte by pages 32768
[ 2506.300065] ex_mempool: mempool: SUCCSESS
[ 2506.300090] ex_mempool: mempool: 134217728 byte, 16 ms, type: mempool
[ 2506.300093] ex_mempool: mempool: 268435456 byte by pages 65536
[ 2506.338836] ex_mempool: mempool: SUCCSESS
[ 2506.338840] ex_mempool: mempool: 268435456 byte, 31 ms, type: mempool
[ 2506.338843] ex_mempool: mempool: 536870912 byte by pages 131072
[ 2506.411588] ex_mempool: mempool: SUCCSESS
[ 2506.411592] ex_mempool: mempool: 536870912 byte, 60 ms, type: mempool
[ 2506.411594] ex_mempool: mempool: 1073741824 byte by pages 262144
[ 2506.555244] ex_mempool: mempool: SUCCSESS
[ 2506.555248] ex_mempool: mempool: 1073741824 byte, 120 ms, type: mempool
[ 2506.555251] ex_mempool: mempool: 2147483648 byte by pages 524288
[ 2506.883730] ex_mempool: mempool: SUCCSESS
[ 2506.883733] ex_mempool: mempool: 2147483648 byte, 280 ms, type: mempool
[ 2506.883735] ex_mempool: mempool: 4294967296 byte by pages 1048576
============================================================
[ 2394.250864] ex_get_pages: get_pages: 4096 byte by pages 1
[ 2394.250867] ex_get_pages: get_pages: SUCCSESS
[ 2394.250868] ex_get_pages: get_pages: 4096 byte, 0 ms, type: mempool
[ 2394.250869] ex_get_pages: get_pages: 8192 byte by pages 2
[ 2394.250871] ex_get_pages: get_pages: SUCCSESS
[ 2394.250872] ex_get_pages: get_pages: 8192 byte, 0 ms, type: mempool
[ 2394.250873] ex_get_pages: get_pages: 16384 byte by pages 4
[ 2394.250875] ex_get_pages: get_pages: SUCCSESS
[ 2394.250876] ex_get_pages: get_pages: 16384 byte, 0 ms, type: mempool
[ 2394.250877] ex_get_pages: get_pages: 32768 byte by pages 8
[ 2394.250880] ex_get_pages: get_pages: SUCCSESS
[ 2394.250881] ex_get_pages: get_pages: 32768 byte, 0 ms, type: mempool
[ 2394.250882] ex_get_pages: get_pages: 65536 byte by pages 16
[ 2394.250888] ex_get_pages: get_pages: SUCCSESS
[ 2394.250888] ex_get_pages: get_pages: 65536 byte, 0 ms, type: mempool
[ 2394.250889] ex_get_pages: get_pages: 131072 byte by pages 32
[ 2394.250900] ex_get_pages: get_pages: SUCCSESS
[ 2394.250900] ex_get_pages: get_pages: 131072 byte, 1 ms, type: mempool
[ 2394.250901] ex_get_pages: get_pages: 262144 byte by pages 64
[ 2394.250920] ex_get_pages: get_pages: SUCCSESS
[ 2394.250921] ex_get_pages: get_pages: 262144 byte, 0 ms, type: mempool
[ 2394.250922] ex_get_pages: get_pages: 524288 byte by pages 128
[ 2394.250965] ex_get_pages: get_pages: SUCCSESS
[ 2394.250966] ex_get_pages: get_pages: 524288 byte, 0 ms, type: mempool
[ 2394.250967] ex_get_pages: get_pages: 1048576 byte by pages 256
[ 2394.251056] ex_get_pages: get_pages: SUCCSESS
[ 2394.251057] ex_get_pages: get_pages: 1048576 byte, 0 ms, type: mempool
[ 2394.251058] ex_get_pages: get_pages: 2097152 byte by pages 512
[ 2394.251231] ex_get_pages: get_pages: SUCCSESS
[ 2394.251232] ex_get_pages: get_pages: 2097152 byte, 0 ms, type: mempool
[ 2394.251233] ex_get_pages: get_pages: 4194304 byte by pages 1024
[ 2394.251583] ex_get_pages: get_pages: SUCCSESS
[ 2394.251584] ex_get_pages: get_pages: 4194304 byte, 0 ms, type: mempool
[ 2394.251585] ex_get_pages: get_pages: 8388608 byte by pages 2048
[ 2394.252331] ex_get_pages: get_pages: SUCCSESS
[ 2394.252333] ex_get_pages: get_pages: 8388608 byte, 1 ms, type: mempool
[ 2394.252335] ex_get_pages: get_pages: 16777216 byte by pages 4096
[ 2394.255706] ex_get_pages: get_pages: SUCCSESS
[ 2394.255711] ex_get_pages: get_pages: 16777216 byte, 3 ms, type: mempool
[ 2394.255713] ex_get_pages: get_pages: 33554432 byte by pages 8192
[ 2394.259862] ex_get_pages: get_pages: SUCCSESS
[ 2394.259866] ex_get_pages: get_pages: 33554432 byte, 4 ms, type: mempool
[ 2394.259868] ex_get_pages: get_pages: 67108864 byte by pages 16384
[ 2394.268049] ex_get_pages: get_pages: SUCCSESS
[ 2394.268054] ex_get_pages: get_pages: 67108864 byte, 6 ms, type: mempool
[ 2394.268056] ex_get_pages: get_pages: 134217728 byte by pages 32768
[ 2394.284426] ex_get_pages: get_pages: SUCCSESS
[ 2394.284429] ex_get_pages: get_pages: 134217728 byte, 13 ms, type: mempool
[ 2394.284431] ex_get_pages: get_pages: 268435456 byte by pages 65536
[ 2394.318342] ex_get_pages: get_pages: SUCCSESS
[ 2394.318345] ex_get_pages: get_pages: 268435456 byte, 26 ms, type: mempool
[ 2394.318347] ex_get_pages: get_pages: 536870912 byte by pages 131072
[ 2394.381492] ex_get_pages: get_pages: SUCCSESS
[ 2394.381496] ex_get_pages: get_pages: 536870912 byte, 48 ms, type: mempool
[ 2394.381498] ex_get_pages: get_pages: 1073741824 byte by pages 262144
[ 2394.506623] ex_get_pages: get_pages: SUCCSESS
[ 2394.506627] ex_get_pages: get_pages: 1073741824 byte, 98 ms, type: mempool
[ 2394.506629] ex_get_pages: get_pages: 2147483648 byte by pages 524288
[ 2394.799637] ex_get_pages: get_pages: SUCCSESS
[ 2394.799640] ex_get_pages: get_pages: 2147483648 byte, 240 ms, type: mempool
[ 2394.799643] ex_get_pages: get_pages: 4294967296 byte by pages 1048576
====================================================================
[ 3239.411969] ex_kmem_cache: kmem_cache: 4096 byte by pages 1
[ 3239.411972] ex_kmem_cache: kmem_cache: SUCCSESS
[ 3239.411972] ex_kmem_cache: kmem_cache: 4096 byte, 0 ms, type: mempool
[ 3239.411974] ex_kmem_cache: kmem_cache: 8192 byte by pages 2
[ 3239.411976] ex_kmem_cache: kmem_cache: SUCCSESS
[ 3239.411977] ex_kmem_cache: kmem_cache: 8192 byte, 0 ms, type: mempool
[ 3239.411978] ex_kmem_cache: kmem_cache: 16384 byte by pages 4
[ 3239.411980] ex_kmem_cache: kmem_cache: SUCCSESS
[ 3239.411980] ex_kmem_cache: kmem_cache: 16384 byte, 0 ms, type: mempool
[ 3239.411982] ex_kmem_cache: kmem_cache: 32768 byte by pages 8
[ 3239.411984] ex_kmem_cache: kmem_cache: SUCCSESS
[ 3239.411985] ex_kmem_cache: kmem_cache: 32768 byte, 0 ms, type: mempool
[ 3239.411986] ex_kmem_cache: kmem_cache: 65536 byte by pages 16
[ 3239.411991] ex_kmem_cache: kmem_cache: SUCCSESS
[ 3239.411992] ex_kmem_cache: kmem_cache: 65536 byte, 0 ms, type: mempool
[ 3239.411993] ex_kmem_cache: kmem_cache: 131072 byte by pages 32
[ 3239.412007] ex_kmem_cache: kmem_cache: SUCCSESS
[ 3239.412008] ex_kmem_cache: kmem_cache: 131072 byte, 0 ms, type: mempool
[ 3239.412009] ex_kmem_cache: kmem_cache: 262144 byte by pages 64
[ 3239.412031] ex_kmem_cache: kmem_cache: SUCCSESS
[ 3239.412031] ex_kmem_cache: kmem_cache: 262144 byte, 0 ms, type: mempool
[ 3239.412033] ex_kmem_cache: kmem_cache: 524288 byte by pages 128
[ 3239.412077] ex_kmem_cache: kmem_cache: SUCCSESS
[ 3239.412078] ex_kmem_cache: kmem_cache: 524288 byte, 0 ms, type: mempool
[ 3239.412079] ex_kmem_cache: kmem_cache: 1048576 byte by pages 256
[ 3239.412173] ex_kmem_cache: kmem_cache: SUCCSESS
[ 3239.412173] ex_kmem_cache: kmem_cache: 1048576 byte, 0 ms, type: mempool
[ 3239.412175] ex_kmem_cache: kmem_cache: 2097152 byte by pages 512
[ 3239.412381] ex_kmem_cache: kmem_cache: SUCCSESS
[ 3239.412381] ex_kmem_cache: kmem_cache: 2097152 byte, 0 ms, type: mempool
[ 3239.412383] ex_kmem_cache: kmem_cache: 4194304 byte by pages 1024
[ 3239.412795] ex_kmem_cache: kmem_cache: SUCCSESS
[ 3239.412797] ex_kmem_cache: kmem_cache: 4194304 byte, 0 ms, type: mempool
[ 3239.412798] ex_kmem_cache: kmem_cache: 8388608 byte by pages 2048
[ 3239.413695] ex_kmem_cache: kmem_cache: SUCCSESS
[ 3239.413697] ex_kmem_cache: kmem_cache: 8388608 byte, 1 ms, type: mempool
[ 3239.413699] ex_kmem_cache: kmem_cache: 16777216 byte by pages 4096
[ 3239.417047] ex_kmem_cache: kmem_cache: SUCCSESS
[ 3239.417052] ex_kmem_cache: kmem_cache: 16777216 byte, 3 ms, type: mempool
[ 3239.417054] ex_kmem_cache: kmem_cache: 33554432 byte by pages 8192
[ 3239.421315] ex_kmem_cache: kmem_cache: SUCCSESS
[ 3239.421318] ex_kmem_cache: kmem_cache: 33554432 byte, 4 ms, type: mempool
[ 3239.421320] ex_kmem_cache: kmem_cache: 67108864 byte by pages 16384
[ 3239.430049] ex_kmem_cache: kmem_cache: SUCCSESS
[ 3239.430052] ex_kmem_cache: kmem_cache: 67108864 byte, 8 ms, type: mempool
[ 3239.430055] ex_kmem_cache: kmem_cache: 134217728 byte by pages 32768
[ 3239.446429] ex_kmem_cache: kmem_cache: SUCCSESS
[ 3239.446432] ex_kmem_cache: kmem_cache: 134217728 byte, 13 ms, type: mempool
[ 3239.446434] ex_kmem_cache: kmem_cache: 268435456 byte by pages 65536
[ 3239.483165] ex_kmem_cache: kmem_cache: SUCCSESS
[ 3239.483169] ex_kmem_cache: kmem_cache: 268435456 byte, 28 ms, type: mempool
[ 3239.483172] ex_kmem_cache: kmem_cache: 536870912 byte by pages 131072
[ 3239.550121] ex_kmem_cache: kmem_cache: SUCCSESS
[ 3239.550125] ex_kmem_cache: kmem_cache: 536870912 byte, 53 ms, type: mempool
[ 3239.550126] ex_kmem_cache: kmem_cache: 1073741824 byte by pages 262144
[ 3239.675628] ex_kmem_cache: kmem_cache: SUCCSESS
[ 3239.675631] ex_kmem_cache: kmem_cache: 1073741824 byte, 102 ms, type: mempool
[ 3239.675634] ex_kmem_cache: kmem_cache: 2147483648 byte by pages 524288
[ 3239.924847] ex_kmem_cache: kmem_cache: SUCCSESS
[ 3239.924851] ex_kmem_cache: kmem_cache: 2147483648 byte, 204 ms, type: mempool
[ 3239.924853] ex_kmem_cache: kmem_cache: 4294967296 byte by pages 1048576