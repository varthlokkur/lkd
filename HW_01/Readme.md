Скомпилируйте собственную оптимизированную версию ядра на виртуальной машине


## Цель
Скомпилировать и настроить оптимизированное ядро для дальнейшей работы


## Описание
- Установить qemu-kvm, virt-manager
- Скачать debian 12 из нашего репозиторя git.
- Установить debian12 на виртуальную машину qemu-kvm.
- Клонировать ядро из нашего репозитория git
- Изменить .config файл, убрав лишние опции и добавив нужные опции для отладки.
- Собрать .deb пакет ядра
- Установить .deb пакет ядра и проверить что все работает


## Формат сдачи
- Cсылка на git репозиторий с проектом HW_01_build_kernel
- в проекте должен быть .deb собраного ядра и .config файл.


## Критерии оценки
- Есть .deb файл ядра.
- Есть .config файл.
- В .config файле есть все необходимые опции для отладки.
- Ядро собрано по отправленному .config файлу.
- Ядро из .deb пакета устанавливается и работает в графике.
- Статус "Принято" ставится при выполнении всех критериев


## Инструкция
- выкачать исходники ядра
``` shel
curl -O https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.1.130.tar.gz
```
- распаковывать
``` shell
tar -xvf linux-6.1.130.tar.gz
```
- выполнить начальную конфигурацию
```shell
cd linux-6.1.130
make localmodconfig
```
- поправить конфигурационный файл (список опций ниже)
``` shell
vim .config
```
- запустить сборку
``` shell
make -j2 deb-pkg 2>error.log
```
- установить пакеты
``` shell
dpkg -i linux-image-6.1.130_*_amd64.deb
dpkg -i linux-headers-6.1.130_*_amd64.deb
dpkg -i linux-libc-6.1.130_*_amd64.deb
```
- перезагрузить машину
``` shell
reboot
```

### Список параметров

- CONFIG_SECURITY_SELINUX=n
- CONFIG_SECURITY_SMACK=n
- CONFIG_SECURITY_TOMOYO=n
- CONFIG_SECURITY_APPARMOR=n
- CONFIG_SECURITY_YAMA=n
- CONFIG_RANDOMIZE_BASE=n
- CONFIG_CPU_MITIGATIONS=n
- CONFIG_MITIGATION_SPECTRE_BHI=n
- CONFIG_MITIGATION_RFDS=n
- CONFIG_PAGE_TABLE_ISOLATION=n
- CONFIG_ZSWAP=n
- CONFIG_BPF=n
- CONFIG_BPF_SYSCALL=n
- CONFIG_BPF_JIT=n
- CONFIG_BPF_EVENTS=n
- CONFIG_BPFILTER=n
- CONFIG_DEBUG_FS=y
- CONFIG_FTRACE=y
- CONFIG_FUNCTION_TRACER=y
- CONFIG_DYNAMIC_FTRACE=y
- CONFIG_FUNCTION_GRAPH_TRACER=y
- CONFIG_STACK_TRACER=y
- CONFIG_KUNIT=y
- CONFIG_KUNIT_TEST=y
- CONFIG_KASAN=y
- CONFIG_STACKTRACE=y
- CONFIG_KASAN_GENERIC=y
- CONFIG_KASAN_INLINE=y
- CONFIG_KASAN_EXTRA_INFO=y
- CONFIG_KGDB=y
- CONFIG_KGDB_SERIAL_CONSOLE=y
- CONFIG_KGDB_KDB=y
- CONFIG_KDB_KEYBOARD=y
- CONFIG_DEBUG_INFO=y
- CONFIG_SERIAL_CONSOLE=y
- CONFIG_SERIAL_CORE_CONSOLE=y
- CONFIG_FRAME_POINTER=y
- CONFIG_CONSOLE_POLL=y
- CONFIG_KPROBES=y
- CONFIG_KPROBE_EVENT=y


## Примечания
При сборке команда `make deb-pkg` меняет конфиг в папке со сборкой и часть параметров переходит в not set.
В репозитории оставил два конфига
- config - тот, который получается после запуска сборки `make deb-pkg`
- config_base - тот, который был до запуска команды `make deb-pkg`