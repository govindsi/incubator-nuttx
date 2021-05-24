/****************************************************************************
 * arch/risc-v/src/k210/k210_i2c.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#ifdef CONFIG_K210_I2C

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <debug.h>
#include <time.h>

#include <nuttx/arch.h>
#include <nuttx/irq.h>
#include <nuttx/clock.h>
#include <nuttx/semaphore.h>
#include <nuttx/i2c/i2c_master.h>

#include <arch/board/board.h>

#include "k210_i2c.h"

#include "riscv_arch.h"

/* I2C state */

enum k210_i2cstate_e
{
  I2CSTATE_IDLE = 0,
  I2CSTATE_PROC,
  I2CSTATE_STOP,
  I2CSTATE_FINISH,
  I2CSTATE_ERROR
};

/* I2C Device hardware configuration */

struct k210_i2c_config_s
{
  uint32_t clk_freq;          /* Clock frequency */

  uint8_t scl_pin;            /* GPIO configuration for SCL as SCL */
  uint8_t sda_pin;            /* GPIO configuration for SDA as SDA */

  uint8_t periph;             /* Peripheral ID */
  uint8_t irq;                /* Interrupt ID */

  uint32_t clk_bit;           /* Clock enable bit */
  uint32_t rst_bit;           /* I2C reset bit */

  uint32_t scl_insig;         /* I2C SCL input signal index */
  uint32_t scl_outsig;        /* I2C SCL output signal index */

  uint32_t sda_insig;         /* I2C SDA input signal index */
  uint32_t sda_outsig;        /* I2C SDA output signal index */
};

/* I2C Device Private Data */

struct k210_i2c_priv_s
{
  const struct i2c_ops_s *ops; /* Standard I2C operations */
  uint32_t id;                 /* I2C instance */

  /* Port configuration */

  const struct k210_i2c_config_s *config;
  int refs;                    /* Reference count */
  sem_t sem_excl;              /* Mutual exclusion semaphore */
  sem_t sem_isr;               /* Interrupt wait semaphore */

  /* I2C work state */

  enum k210_i2cstate_e i2cstate;
  struct i2c_msg_s *msgv;      /* Message list */
  uint8_t msgid;               /* Current message ID */
  ssize_t bytes;               /* Processed data bytes */
  uint32_t error;              /* I2C transform error */
  bool ready_read;             /* If I2C is ready for receiving data */
  uint32_t clk_freq;           /* Current I2C Clock frequency */
};

static struct k210_i2c_priv_s k210_i2c0_priv =
{
  .ops        = NULL,
  .id         = 0,
  .config     = NULL,
  .refs       = 0,
  .i2cstate   = I2CSTATE_IDLE,
  .msgv       = NULL,
  .msgid      = 0,
  .bytes      = 0,
  .error      = 0,
  .ready_read = false,
  .clk_freq   = 0
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: k210_i2cbus_initialize
 *
 * Description:
 *   Initialize the selected I2C port. And return a pointer to an unique
 *   instance of struct i2c_master_s. This function may be called to obtain
 *   multiple instances of the interface, each of which may be set up with a
 *   different frequency and slave address.
 *
 * Parameters:
 *   port          - Port number of the I2C interface to be initialized.
 *
 * Returned Value:
 *   Pointer to valid I2C device structure is returned on success.
 *   A NULL pointer is returned on failure.
 *
 ****************************************************************************/

struct i2c_master_s *k210_i2cbus_initialize(int port)
{
  int ret;
  irqstate_t flags;
  struct k210_i2c_priv_s *priv;
  const struct k210_i2c_config_s *config;

  switch (port)
    {
#ifdef CONFIG_K210_I2C0
    case 0:
      priv = &k210_i2c0_priv;
      break;
#endif
    default:
      return NULL;
    }

  config = priv->config;

  flags = enter_critical_section();

  if ((volatile int)priv->refs++ != 0)
    {
      leave_critical_section(flags);

      i2cinfo("Returning previously initialized I2C bus. "
              "Handler: %" PRIxPTR "\n", (uintptr_t)priv);

      return (struct i2c_master_s *)priv;
    }

  leave_critical_section(flags);

  i2cinfo("I2C bus initialized! Handler: %" PRIxPTR "\n", (uintptr_t)priv);

  return (struct i2c_master_s *)priv;
}

/****************************************************************************
 * Name: k210_i2cbus_uninitialize
 *
 * Description:
 *   De-initialize the selected I2C port and power down the device.
 *
 * Parameters:
 *   dev           - Device structure as returned by
 *                   k210_i2cbus_initialize()
 *
 * Returned Value:
 *   OK is returned on success. ERROR is returned when internal reference
 *   count mismatches or dev points to invalid hardware device.
 *
 ****************************************************************************/

int k210_i2cbus_uninitialize(struct i2c_master_s *dev)
{
  irqstate_t flags;
  struct k210_i2c_priv_s *priv = (struct k210_i2c_priv_s *)dev;

  DEBUGASSERT(dev);

  if (priv->refs == 0)
    {
      return ERROR;
    }

  flags = enter_critical_section();

  if (--priv->refs)
    {
      leave_critical_section(flags);
      return OK;
    }

  leave_critical_section(flags);

  return OK;
}

#endif /* CONFIG_K210_I2C */
