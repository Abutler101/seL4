/*
 * Copyright 2014, General Dynamics C4 Systems
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(GD_GPL)
 */

#include <config.h>
#include <object.h>
#include <machine.h>
#include <arch/model/statedata.h>
#include <arch/kernel/vspace.h>
#include <arch/kernel/thread.h>
#include <linker.h>

void Arch_switchToThread(tcb_t *tcb)
{
    setVMRoot(tcb);
#if defined(CONFIG_KERNEL_GLOBALS_FAME)
    *armKSGlobalsFrame = tcb->tcbIPCBuffer;
    armKSGlobalsFrame[1] = getRegister(tcb, TLS_BASE);
#endif
    clearExMonitor();
}

BOOT_CODE void Arch_configureIdleThread(tcb_t *tcb)
{
    setRegister(tcb, CPSR, CPSR_IDLETHREAD);
    setRegister(tcb, NextIP, (word_t)idleThreadStart);
}

void Arch_switchToIdleThread(void)
{
    if (config_set(CONFIG_ARM_HYPERVISOR_SUPPORT)) {
        vcpu_switch(NULL);
    }

    /* Force the idle thread to run on kernel page table */
    setVMRoot(NODE_STATE(ksIdleThread));

#ifdef CONFIG_KERNEL_GLOBALS_FAME
    *armKSGlobalsFrame = 0;
    armKSGlobalsFrame[1] = 0;
#endif /* CONFIG_KERNEL_GLOBALS_FAME */
}

void Arch_activateIdleThread(tcb_t *tcb)
{
    /* Don't need to do anything */
}
