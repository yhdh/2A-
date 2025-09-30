# Dans une VM/container Linux avec privilèges suffisants :
echo 200 | sudo tee /proc/sys/vm/nr_hugepages
sudo mkdir -p /mnt/huge
sudo mount -t hugetlbfs -o mode=0777 none /mnt/huge
