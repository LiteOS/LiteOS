####Kernel command####
#task_shellcmd.c -utask_shellcmd
#cpup_shellcmd.c -ucpup_shellcmd
#shell_shellcmd.c -uhelp_shellcmd
#mempt_shellcmd.c -ufree_shellcmd -umemcheck_shellcmd -uuname_shellcmd -ureadreg_shellcmd -uwritereg_shellcmd
#sem_shellcmd.c -usem_shellcmd
#sysinfo_shellcmd.c -usysteminfo_shellcmd
#swtmr_shellcmd.c -uswtmr_shellcmd
#hwi_shell.c -uhwi_shellcmd
#dynload_shellcmd.c -uldinit_shellcmd -ucall_shellcmd -ufindsym_shellcmd -ulddrop_shellcmd -umclose_shellcmd -umopen_shellcmd
LITEOS_TABLES_KERNEL_LDFLAGS := \
    -utask_shellcmd \
    -ucpup_shellcmd \
    -uhelp_shellcmd \
    -ufree_shellcmd \
    -usem_shellcmd \
    -umutex_shellcmd \
    -uqueue_shellcmd \
    -usysteminfo_shellcmd \
    -uswtmr_shellcmd \
    -uhwi_shellcmd \
    -ufindsym_shellcmd \
    -uzbar_scan_shellcmd \
    -udmesg_shellcmd \
    -udate_shellcmd \
    -ureadExcInfo_shellcmd \
    -uwatch_shellcmd \
    -udeadlock_shellcmd


####Net command####
#api_shell.c -uarp_shellcmd -uifconfig_shellcmd -uping_shellcmd -utftp_shellcmd -unetstat_shellcmd -udns_shellcmd -untpdate_shellcmd
#telnet_loop.c -utelnet_shellcmd
LITEOS_TABLES_NET_LDFLAGS := \
    -uarp_shellcmd \
    -utelnet_shellcmd

####FS command####
#vfs_shellcmd.c -ucd_shellcmd -ucat_shellcmd -uls_shellcmd -upwd_shellcmd -urm_shellcmd -urmdir_shellcmd -umkdir_shellcmd -utouch_shellcmd -uumount_shellcmd -umount_shellcmd -usync_shellcmd -ustatfs_shellcmd -ucp_shellcmd
#fat_shellcmd.c -uformat_shellcmd
#mtd_shellcmd.c -upartition_shellcmd
#proc_shellcmd.c -uwriteproc_shellcmd
#disk_shellcmd.c -upartinfo_shellcmd
LITEOS_TABLES_FS_LDFLAGS := \
    -ucd_shellcmd \
    -uformat_shellcmd \
    -upartition_shellcmd \
    -uwriteproc_shellcmd \
    -upartinfo_shellcmd \
    -uumount_shellcmd -umount_shellcmd \
    -uvirstatfs_shellcmd \
    -ulsfd_shellcmd \
    -ufatfsck_shellcmd

####Tools command####
#iperf_shell.c -uiperf_shellcmd
LITEOS_TABLES_TOOLS_LDFLAGS := \
    -uiperf_shellcmd \
    -ulwip_dump_shellcmd

####Extend command####
#reset_shell.c -ureset_shellcmd
#bcm_wifi.c -ustartap_shellcmd -ustopap_shellcmd -ubcmwifi_shellcmd
#misc_shell.c -uhimd_shellcmd -uhimm_shellcmd
#ddrs_shell.c -uhiddrs_shellcmd
#nand_shell.c -unand_bad_shellcmd
#i2c_shell.c -ui2c_read_shellcmd -ui2c_write_shellcmd
#spi_shell.c -ussp_read_shellcmd -ussp_write_shellcmd
#uart_shell.c -uuart_config_shellcmd -uuart_write_shellcmd -uuart_read_shellcmd -uuart_close_shellcmd
LITEOS_TABLES_EXTEND_LDFLAGS := \
    -ureset_shellcmd \
    -ustartap_shellcmd \
    -uhimd_shellcmd \
    -uhiddrs_shellcmd \
    -unand_bad_shellcmd \
    -ui2c_read_shellcmd \
    -ussp_read_shellcmd \
    -uuart_config_shellcmd\
    -uusb_debug_shellcmd

LITEOS_TABLES_DRIVER_LDFLAGS := \
    -ui2c_init \
    -ugpio_init \
    -uregulator_init \
    -umtd_init_list \
    -uhispi_init \
    -uhifmc100_init \
    -uhisfc350_init \
    -unand_hifmc100_init \
    -uhifmc100_parallel_init \
    -usd_mci_init \
    -uhi_mci_init \
    -upl011_init \
    -uhinfc620_init \
    -uhisnfc100_init \
    -uregulator_machine_init \
    -uhisimeidia_regulator_init \
    -ucpufreq_init  \
    -uhisilicon_cpufreq_init \
    -ucpufreq_machine_init  \
    -udevfreq_init \
    -umedia_devfreq_init \
    -udevfreq_machine_init\
    -uhieth_machine_init\
    -uhigmac_machine_init

####FS MAP####
#fs_mount.c -ug_fsmap_wow -ug_fsmap_wow_end
LITEOS_TABLES_WOW_LDFLAGS := \
    $(LITEOS_TABLES_DRIVER_LDFLAGS) \
    -ujffs_fsmap \
    -uprocfs_fsmap \
    -ug_fsmap_wow

#fs_mount.c -ug_fsmap_scatter -ug_fsmap_scatter_end
LITEOS_TABLES_SCATTER_LDFLAGS := \
    $(LITEOS_TABLES_DRIVER_LDFLAGS) \
    -ug_fsmap_scatter

LITEOS_TABLES_FSMAP_LDFLAGS := \
    -uramfs_fsmap \
    -unfs_fsmap \
    -ufat_fsmap \
    -uyaffs_fsmap \
    -uromfs_fsmap \
    -ug_fsmap

LITEOS_TABLES_LDFLAGS := \
    $(LITEOS_TABLES_KERNEL_LDFLAGS)\
    $(LITEOS_TABLES_NET_LDFLAGS) \
    $(LITEOS_TABLES_FS_LDFLAGS) \
    $(LITEOS_TABLES_TOOLS_LDFLAGS) \
    $(LITEOS_TABLES_EXTEND_LDFLAGS) \
    $(LITEOS_TABLES_FSMAP_LDFLAGS) \
    $(LITEOS_TABLES_WOW_LDFLAGS) \
    $(LITEOS_TABLES_SCATTER_LDFLAGS) \
    $(LITEOS_TABLES_DRIVER_LDFLAGS)
