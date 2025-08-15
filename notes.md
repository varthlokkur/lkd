установка clang-format

sudo apt update
sudo apt install clang-format-19

Запись в переменную для модуля
echo value | sudo tee /sys/module/module_name/parameters/parameter_name

Скачать данные для ядра
git clone git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git
cd linux
git checkout 6ae7ac5c4251

curl -O https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.1.130.tar.gz
tar -xvf linux-6.1.130.tar.gz

ERROR: modpost: "open_softirq" [/home/student/workspace/HW_05/src/ex_softirq.ko] undefined!
