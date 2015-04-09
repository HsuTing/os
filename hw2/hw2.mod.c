#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xe00b4984, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x5afe1292, __VMLINUX_SYMBOL_STR(single_release) },
	{ 0xe887354a, __VMLINUX_SYMBOL_STR(seq_read) },
	{ 0x7c4c2659, __VMLINUX_SYMBOL_STR(seq_lseek) },
	{ 0x5d41c87c, __VMLINUX_SYMBOL_STR(param_ops_charp) },
	{ 0x945e4c08, __VMLINUX_SYMBOL_STR(remove_proc_entry) },
	{ 0x50eedeb8, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x99c316ce, __VMLINUX_SYMBOL_STR(proc_create_data) },
	{ 0xfd485ec8, __VMLINUX_SYMBOL_STR(seq_printf) },
	{ 0xc28522e1, __VMLINUX_SYMBOL_STR(init_task) },
	{ 0x990395ac, __VMLINUX_SYMBOL_STR(single_open) },
	{ 0xb4390f9a, __VMLINUX_SYMBOL_STR(mcount) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "1B52D33F4D0C9B8DF89799A");
