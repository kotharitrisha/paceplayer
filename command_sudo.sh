sudo chmod 777 -R /sys/kernel/debug/
sudo chmod 777 /sys/kernel/debug/omap_mux/spi0_d0
sudo chmod 777 /sys/kernel/debug/omap_mux/spi0_sclk

echo 1 > /sys/kernel/debug/omap_mux/spi0_d0
echo 21 > /sys/kernel/debug/omap_mux/spi0_sclk

echo "UART 2 (TX):"
cat /sys/kernel/debug/omap_mux/spi0_d0
echo

echo "UART 2 (RX):"
cat /sys/kernel/debug/omap_mux/spi0_sclk
echo

sudo chmod 777  /dev/ttyO2
stty -F /dev/ttyO2 raw
stty -F /dev/ttyO2 -echo
