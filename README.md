# xv6-riscv — Proyecto Final de Sistemas Operativos

Modificación del sistema operativo educativo xv6-riscv como proyecto
final del curso de Sistemas Operativos.

**Estudiante:** Alejandro Cardona Arellano  
**Institución:** Universidad del Valle — Escuela de Sistemas y Computación  
**Docente:** Jefferson Amado Peña Torres  
**Fecha:** Julio de 2026

---

## Modificaciones implementadas

### 1. Planificador por prioridades con envejecimiento (*aging*)

**Archivos modificados:** `kernel/param.h`, `kernel/proc.h`, `kernel/proc.c`,
`kernel/sysproc.c`, `kernel/syscall.h`, `kernel/syscall.c`,
`user/user.h`, `user/usys.pl`

El planificador original de xv6 implementa *round-robin* sin diferenciación
entre procesos. Esta modificación introduce:

- Un campo `priority` (0 = alta, 9 = baja) y un campo `wait_ticks` en
  `struct proc`.
- El `scheduler()` selecciona en cada ciclo el proceso `RUNNABLE` de menor
  valor de prioridad, en lugar del primero encontrado en el arreglo.
- Un mecanismo de *aging*: un proceso que permanece `RUNNABLE` durante
  `AGING_THRESHOLD` iteraciones sin ser elegido incrementa automáticamente
  su prioridad, evitando inanición indefinida.
- Una nueva syscall `setpriority(pid, priority)` que permite a un proceso
  padre fijar la prioridad de un proceso hijo.

Constantes definidas en `kernel/param.h`:

```c
#define DEFAULT_PRIORITY  5
#define MIN_PRIORITY      0
#define MAX_PRIORITY      9
#define AGING_THRESHOLD  30
```

### 2. Asignación perezosa de memoria (*lazy allocation*)

**Archivos modificados:** `kernel/proc.c`, `kernel/trap.c`, `kernel/vm.c`,
`kernel/kalloc.c`, `kernel/sysproc.c`, `kernel/syscall.h`,
`kernel/syscall.c`, `user/user.h`, `user/usys.pl`

En xv6 original, `sbrk()` reserva páginas físicas de inmediato al crecer
el heap de un proceso. Esta modificación implementa asignación bajo demanda:

- `growproc()` en `kernel/proc.c` solo actualiza el tamaño lógico del
  proceso (`p->sz`) sin llamar a `uvmalloc()`.
- `usertrap()` en `kernel/trap.c` detecta fallos de página de carga
  (*scause* = 13) y de escritura (*scause* = 15) dentro del rango lógico
  válido del proceso, y en ese momento llama a `kalloc()` + `mappages()`
  para asignar la página física.
- `uvmunmap()` y `uvmcopy()` en `kernel/vm.c` fueron modificados para
  tolerar páginas del rango lógico del proceso que aún no tienen página
  física asociada (en lugar de hacer `panic`).
- Se agregó la syscall de instrumentación `freepages()` en
  `kernel/kalloc.c`, que cuenta las páginas actualmente disponibles en la
  lista libre, para poder medir el consumo de memoria física durante las
  pruebas.

---

## Programas de prueba incluidos

| Programa | Descripción |
|---|---|
| `user/testpriority.c` | Lanza 3 procesos con prioridades 0, 5 y 9; mide el tiempo de ejecución de cada uno con `uptime()`. |
| `user/membench.c` | Solicita 40MB con `sbrk()` y mide páginas físicas consumidas antes, inmediatamente después, y tras tocar 10 páginas. |
| `user/lazytest.c` | Solicita 200MB lógicos (más que la RAM física disponible), toca solo algunas páginas y verifica que el sistema no falle. |

---

## Requisitos del entorno

| Componente | Versión utilizada |
|---|---|
| Sistema operativo host | Windows 11 + WSL2 (versión 2.6.3.0) |
| Distribución Linux | Ubuntu 26.04 LTS |
| Compilador cruzado | `riscv64-linux-gnu-gcc` 15.2.0 |
| Emulador | `qemu-system-riscv64` 10.2.1 |
| Depurador | `gdb-multiarch` |

---

## Instalación de dependencias

```bash
sudo apt update
sudo apt install git build-essential gdb-multiarch qemu-system-riscv \
                 gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu
```

---

## Compilación y ejecución

```bash
# Clonar el repositorio
git clone https://github.com/aca2211/xv6-os-final-project.git
cd xv6-os-final-project

# Compilar y ejecutar (configuración por defecto: 3 harts, 128MB RAM)
make qemu

# Para correr pruebas del planificador (1 hart, fuerza contención real)
make qemu CPUS=1
```

Dentro del shell de xv6:
$ testpriority     # prueba del planificador por prioridades
$ membench         # prueba de asignación perezosa (cuantitativa)
$ lazytest         # prueba de asignación perezosa (caso límite 200MB)

Para salir de QEMU: `Ctrl+a`, luego `x`.

---

## Estructura del repositorio
kernel/     Código fuente del kernel (modificado)
user/       Programas de usuario (incluye los programas de prueba)
mkfs/       Herramienta para construir la imagen del sistema de archivos
Makefile    Automatiza la compilación y ejecución
README      README original de xv6-riscv (MIT)
README.md   Este archivo — documentación del proyecto
---

## Repositorio upstream

Este proyecto está basado en [mit-pdos/xv6-riscv](https://github.com/mit-pdos/xv6-riscv).  
El remoto `upstream` apunta al repositorio original del MIT para referencia comparativa.
