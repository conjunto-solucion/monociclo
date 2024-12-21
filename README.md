## Compilación

En el directorio principal:

```
$ g++ -o ./monociclo $(find ./src/ -name "*.cpp")
```

## Uso

```
$ monociclo <fuente> [salida]
```

* fuente: es un archivo de texto con un programa en el lenguaje monociclo

* salida (opcional): es la ruta y nombre de archivo a crear.

Ejemplo:

`$ monociclo ./ejemplos/factorial.mono ./ejemplo.txt`

Traduce el programa factorial.mono a texto hexadecimal y lo guarda en ejemplo.txt. Este archivo es apto para su uso en Logisim Evolution (probado para la versión 2.13.8).


# Descripción general

Arquitectura mínima de un procesador monociclo de 16 bits. Incluye traductor de programas a texto en hexadecimal.


## Formatos de instrucción

Referencias:

* S1: registro fuente 1
* S2: registro fuente 2
* D: registro destino
* opcode: código de operación
* I: valor inmediato, I ∈ [-7, 7]
* i0: bits 12 a 15
* i1: bits 8 a 11
* i2: bits 4 a 7
* i3: bits 0 a 3

Las instrucciones tienen una longitud fija de 16 bits, y están segmentadas en bloques de 4 bits. Se distinguen tres formatos de instrucción:

**Formato R**

Para instrucciones que, además del código de operación, sólo contienen direcciones de registros.


|i3|i2|i1|i0|
|:--------:|:--------:|:---------:|:----------:|
|    S2    |    S1    |     D     |   opcode   |

**Formato I**

Para instrucciones que contienen valores inmediatos y almacenan valores en un registro.


|i3|i2|i1|i0|
|:--------:|:--------:|:---------:|:----------:|
|    I     |    S1    |     D     |   opcode   |


**Formato J**

Para instrucciones que contienen valores inmediatos, no almacenan valores en registros, y pueden desencadenar un cambio en el contador del programa distinto del habitual.


|i3|i2|i1|i0|
|:--------:|:--------:|:---------:|:----------:|
|    S2    |    S1    |     I     |   opcode   |


## Instrucciones

**not**
- Formato: R
- Sintaxis: `not D S1 r0`
- RTL: R[D] ← ¬(R[S1] + R[r0]); PC ← PC + 1;
- Descripción: almacena en el registro D el valor ¬R[S1]
- Código: 0000

**\+**
- Formato: R
- Sintaxis: `+ D S1 S2`
- RTL: R[D]←R[S1]+R[S2]; PC←PC+1;;
- Descripción: almacena en el registro D el valor R[S1]+R[S2]
- Código: 0001

**\-**
- Formato: R
- Sintaxis: `- D S1 S2`
- RTL: R[D]←R[S1]−R[S2]; PC←PC+1;
- Descripción: almacena en el registro D el valor R[S1]−R[S2]
- Código: 0010

**or**
- Formato: R
- Sintaxis: `or D S1 S2`
- RTL: R[D]←R[S1] | R[S2]; PC←PC+1;
- Descripción: almacena en el registro D el valor R[S1] ∨ R[S2]
- Código: 0011

**and**
- Formato: R
- Sintaxis: `and D S1 S2`
- RTL: R[D]←R[S1]&R[S2]; PC←PC+1;
- Descripción: almacena en el registro D el valor de R[S1] Λ R[S2]
- Código: 0100

**=**
- Formato: J
- Sintaxis: `= I S1 S2 `
- RTL: if (R[S1] == R[S2]) PC←PC+I else PC←PC+1;
- Descripción: suma un inmediato a PC si R[S1] = R[S2]
- Código: 0101

**\>**
- Formato: J
- Sintaxis: `> I S1 S2`
- RTL: if (R[S1] > R[S2]) PC←PC+I else PC←PC+1;
- Descripción: suma un inmediato a PC si (R[S1] > R[S2])
- Código: 0110

**\>=**
- Formato: J
- Sintaxis: `>= I S1 S2`
- RTL: if (R[S1] ≥ R[S2]) PC←PC+I else PC←PC+1;
- Descripción: suma un inmediato a PC si (R[S1] ≥ R[S2])
- Código: 0111

**+i**
- Formato: I
- Sintaxis: `+i D S1 I`
- RTL: R[D]←R[S1]+I; PC←PC+1;
- Descripción: almacena en el registro D el valor R[S1] + I
- Código: 1000

**read**
- Formato: R
- Sintaxis: `read D S1 S2`
- RTL: R[D]←M[R[S1]+R[S2]]; PC←PC+1;
- Descripción: almacena en el registro D el valor en la posición de memoria R[S1]+R[S2]
- Código: 1001

**write**
- Formato: R
- Sintaxis: `write D S1 S2`
- RTL: M[R[S1]+R[S2]]←R[D]; PC←PC+1;
- Descripción: almacena el valor R[D] en la dirección de memoria R[S1]+R[S2]
- Código: 1010

**readi**
- Formato: I
- Sintaxis: `readi D S1 I`
- RTL: R[D]←M[R[S1]+I]; PC←PC+1;
- Descripción: almacena en el registro D el valor en la dirección de memoria R[S1]+I
- Código: 1011

**writei**
- Formato: I
- Sintaxis: `writei D S1 I`
- RTL: M[R[S1]+I]←R[D]; PC←PC+1;
- Descripción: almacena R[D] en la dirección de memoria R[S1]+I
- Código: 1100


## Pseudoinstrucciones

**not**
- Sintaxis: `not S1`
- Descripción: almacena en el registro S1 el valor ¬R[S1]
- Equivale a: `not S1 S1 r0`

**idle**
- Sintaxis: `idle`
- Descripción: espera un ciclo
- Equivale a: `not r0 r0 r0`

**++**
- Sintaxis: `++ S1`
- Descripción: almacena en el registro S1 el valor R[S1]+1
- Equivale a: `+i RF1 RF1 +1`

**--**
- Sintaxis: `-- S1`
- Descripción: almacena en el registro S1 el valor R[S1]−1
- Equivale a: `+i S1 S1 -1`

**move**
- Sintaxis: `move S1 S2`
- Descripción: almacena en el registro S1 el valor R[S2]
- Equivale a: `+ S1 S2 r0`

## Comentarios

Los comentarios comienzan con #. Ejemplo:
```
# esto es una línea de comentario
move r1 r2 # esto es un comentario después de una instrucción
```


## Señales de control


|Instrucción|Opcode|ALU con inmediato|Escribir registro|Escribir memoria|Leer memoria|Permitir salto|Operación ALU|Negar salida|
|:-----:|:-----:|:-:|:--:|:-:|:-:|:-:|:-:|:-:|
|not	|0000	|0	|1		|0	|0	|0	|00	|1  |
|+	    |0001	|0	|1		|0	|0	|0	|00	|0  |
|-	    |0010	|0	|1		|0	|0	|0	|01	|0  |
|or	    |0011	|0	|1		|0	|0	|0	|10	|0  |
|and	|0100	|0	|1		|0	|0	|0	|11	|0  |
|=	    |0101	|0	|0		|0	|0	|1	|X	|0  |
|>	    |0110	|0	|0		|0	|0	|1	|X	|0  |
|>=	    |0111	|0	|0		|0	|0	|1	|X	|0  |
|+i	    |1000	|1	|1		|0	|0	|0	|00	|0  |
|read	|1001	|0	|1		|0	|1	|0	|00	|0  |
|write	|1010	|0	|0		|1	|0	|0	|00	|0  |
|readi	|1011	|1	|1		|0	|1	|0	|00	|0  |
|writei	|1100	|1	|0		|1	|0	|0	|00	|0  |

# Descripción de componentes

El circuito se compone de:
* Un reloj
* Circuito del contador de programa
* Memoria de instrucciones
* Decodificador
* Banco de registros
* ALU
* Memoria de datos

## Reloj

Entradas: ninguna.

Salidas:
* clock: pulso de reloj.

## Contador de programa

El contador de programa es un registro no direccionable de 16 bits conectado a la memoria de instrucciones.

Entradas:
* clock (1 bit): el contador de programa se actualiza con cada pulso de reloj.
* i1 (4 bits): valor inmediato a sumar al contador en una instrucción de salto que cumple con la condición.
* condition_met (1 bit): señal de control para indicar si se cumplió la condición lógica de una instrucción de salto.
* jump_enable (1 bit): señal de control para indicar si la instrucción actual podría desencadenar un salto.

Salidas:
* data (16 bits): valor actual del contador de programa. Siempre legible.

## Memorias
El procesador *monociclo* está diseñado para trabajar con 2 memorias: una memoria de instrucciones y una memoria de datos. Cada dirección de memoria almacena 2 bytes.

Debido a la restricción de tamaño de los registros, de 16 bits, las memorias tienen 65536 direcciones de 0x0000 a 0xFFFF, es decir 65536 bytes o 64 KiB.

El direccionamiento es indirecto por registros (las direcciones se obtienen mediante la suma de dos valores recuperados de registros), excepto en las instrucciones `readi` y `writei`, que tienen direccionamiento indexado preincrementado (la dirección de acceso a memoria se calcula como la suma del contenido de un registro base más una constante inmediata).

### Memoria de instrucciones

Entradas:
* clock (1 bit): la memoria de instrucciones se actualiza con cada subida de la señal de reloj.
* read_address (16 bits): dirección a leer.

Salidas:
* i0 (4 bits): bits 12 a 15 de la instrucción. Corresponde al opcode.
* i1 (4 bits): bits 8 a 11 de la instrucción.
* i2 (4 bits): bits 4 a 7 de la instrucción.
* i3 (4 bits): bits 0 a 3 de la instrucción.

### Memoria de datos

Entradas:
* clock (1 bit): para la escritura la señal de reloj debe ser 0.
* address (16 bits): dirección a leer o escribir.
* write_enable (1 bit): indica si se debe habilitar la escritura de datos.
* read_enable (1 bit): indica si se debe habilitar la lectura de datos.
* write_data (16 bits): datos a escribir. Proviene del registro especificado en i1.

Salidas:
* data (16 bits): dato almacenado en la dirección pasada en address. Si read_enable == 0, devuelve address.

## Decodificador

Entradas:
* i0 (4 bits): opcode a decodificar.

Salidas:
* jump_enable (1 bit): indica si se debe permitir al contador de programa realizar un salto distinto de +1.
* ALU_immediate (1 bit): indica si el valor pasado a la ALU como segundo operando debe interpretarse como un valor inmediato en lugar de un código de registro.
* register_write_enable (1 bit): indica si se debe permitir la escritura en el banco de registros.
* memory_read_enable (1 bit): indica si se debe permitir la lectura de la memoria de datos.
* memory write enable (1 bit): indica si se debe 
permitir la escritura de datos en la memoria de datos.
* negate_output (1 bit): indica si se debe invertir el resultado de la ALU.

## Banco de registros
El procesador soporta exclusivamente números enteros.
Dispone de 16 registros de propósito general de 16 bits cada uno. Los registros de propósito general son:

|Nombre |Código |Nota                      |
|:-----:|:-----:|:------------------------:|
|r0     |0000   |inmutable con valor 0x0000|
|r1	    |0001   |
|r2	    |0010   |
|r3	    |0011   |
|r4	    |0100   |
|r5	    |0101   |
|r6	    |0110   |
|r7	    |0111   |
|r8	    |1000   |
|r9	    |1001   |
|r10    |1010   |
|r11    |1011   |
|r12    |1100   |
|r13    |1101   |
|r14    |1110   |
|r15    |1111   |

Entradas:
* clock (1 bit): los registros se pueden actualizar con cada pulso de reloj.
* register_write_enable (1 bit): indica si se debe permitir la escritura.
* i1 (4 bits): el segundo fragmento de instrucción. Representa el registro destino o un valor inmediato, en cuyo caso es irrelevante.
* i2 (4 bits): el tercer fragmeto de instrucción. Representa el registro fuente 1.
* i3 (4 bits): el cuarto fragmento de instrucción. Representa el registro fuente 2 o un valor inmediato, en cuyo caso es irrelevante.
* write_data (16 bits): el canal para la escritura de datos. Proviene o bien de la memoria de datos o bien de la ALU, dependiendo de la instrucción.

Salidas:
* out_i1 (16 bits): contenido del registro especificado en el fragmento i1 de la instrucción.
* out_i2 (16 bits): contenido del registro especificado en el fragmento i2 de la instrucción.
* out_i3 (16 bits): contenido del registro especificado en el fragmento i3 de la instrucción.


## ALU

Entradas:
* operand_1 (16 bits): dato sobre el cual operar. Proviene de un registro.
* operand_2 (16 bits): dato sobre el cual operar. Proviene de un registro. Cuando ALU_immediate == 1 se usa i3 en su lugar.
* i0 (4 bits): código de operación.
* i3 (4 bits): posible valor inmediato con el cual operar.
* negate_output (1 bit): indica si el resultado debe ser invertido.
* ALU_immediate (1 bit): indica si se debe usar el valor inmediate i3 en lugar del valor de registro operand_2.

Salidas:
* condition_met (1 bit): indica si la condición lógica especificada en la instrucción, si hubiese, se cumplió.
* result (16 bits): resultado de la operación.