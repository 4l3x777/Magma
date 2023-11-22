# GOST ```34.12-2018```|```28147-89``` Magma.

+ ```Intel syntax assembler x64``` for Windows OS realization. 

+ ```Dynamic``` and ```static``` release. 

### Algorithm guide:

![alt text](/img/magma.png)

### How to use:

#### Dynamic:
+ copy ```gost_34_12_magma_dynamic.h```
+ copy ```magma_dynamic.dll```
+ copy ```magma_dynamic.lib```
+ copy ```magma_dynamic.exp```

#### Static:
+ copy ```gost_34_12_magma_static.h```
+ copy ```magma_dynamic.lib```

### Use in your code: 
```C++ 
uint64_t encrypt(uint8_t* byPKey, uint64_t data_block);

uint64_t decrypt(uint8_t* byPKey, uint64_t data_block); 
```

### Testing speed (10Mb):
+ C++/C encryption time: 275 milliseconds
+ ASM encryption time: 253 milliseconds
+ ~ same time





