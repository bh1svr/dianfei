#include "fsl_ctimer.h"
#include "fsl_debug_console.h"
#include "fsl_p3t1755.h"
#include "fsl_i3c.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_MASTER             I3C0
#define I3C_MASTER_CLOCK_FREQUENCY CLOCK_GetI3CFClkFreq()
#define SENSOR_SLAVE_ADDR          0x48U
#define I3C_TIME_OUT_INDEX 100000000U

#define SENSOR_ADDR 0x08U
#define CCC_RSTDAA  0x06U
#define CCC_SETDASA 0x87

#ifndef EXAMPLE_I2C_BAUDRATE
#define EXAMPLE_I2C_BAUDRATE 400000
#endif
#ifndef EXAMPLE_I3C_OD_BAUDRATE
#define EXAMPLE_I3C_OD_BAUDRATE 1500000
#endif
#ifndef EXAMPLE_I3C_PP_BAUDRATE
#define EXAMPLE_I3C_PP_BAUDRATE 4000000
#endif

#define CTIMER          CTIMER1         /* Timer 1 */
#define CTIMER_MAT0_OUT kCTIMER_Match_2 /* Match output 2 */
#define CTIMER_CLK_FREQ CLOCK_GetCTimerClkFreq(1U)
/* Match Configuration for Channel 0 */
static ctimer_match_config_t matchConfig0;
void ctimer_match0_callback(uint32_t flags);
/* Array of function pointers for callback for each channel */
ctimer_callback_t ctimer_callback_table[] = {
    NULL, NULL, ctimer_match0_callback, NULL, NULL, NULL, NULL, NULL};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void i3c_master_callback(I3C_Type *base, i3c_master_handle_t *handle, status_t status, void *userData);
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile status_t g_completionStatus;
volatile bool g_masterCompletionFlag;
i3c_master_handle_t g_i3c_m_handle;
p3t1755_handle_t p3t1755Handle;

const i3c_master_transfer_callback_t masterCallback = {
    .slave2Master = NULL, .ibiCallback = NULL, .transferComplete = i3c_master_callback};

volatile uint32_t random;
uint16_t factor = 1;

void ctimer_match0_callback(uint32_t flags)
{
    random += factor;
}
/*******************************************************************************
 * Code
 ******************************************************************************/
static void i3c_master_callback(I3C_Type *base, i3c_master_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        g_masterCompletionFlag = true;
    }

    g_completionStatus = status;
}

status_t I3C_WriteSensor(uint8_t deviceAddress, uint32_t regAddress, uint8_t *regData, size_t dataSize)
{
    status_t result                  = kStatus_Success;
    i3c_master_transfer_t masterXfer = {0};
    uint32_t timeout                 = 0U;

    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.direction      = kI3C_Write;
    masterXfer.busType        = kI3C_TypeI3CSdr;
    masterXfer.subaddress     = regAddress;
    masterXfer.subaddressSize = 1;
    masterXfer.data           = regData;
    masterXfer.dataSize       = dataSize;
    masterXfer.flags          = kI3C_TransferDefaultFlag;

    g_masterCompletionFlag = false;
    g_completionStatus     = kStatus_Success;
    result                 = I3C_MasterTransferNonBlocking(EXAMPLE_MASTER, &g_i3c_m_handle, &masterXfer);
    if (kStatus_Success != result)
    {
        return result;
    }

    while (!g_masterCompletionFlag)
    {
        timeout++;
        if ((g_completionStatus != kStatus_Success) || (timeout > I3C_TIME_OUT_INDEX))
        {
            break;
        }
    }

    if (timeout == I3C_TIME_OUT_INDEX)
    {
        result = kStatus_Timeout;
    }
    result = g_completionStatus;

    return result;
}

status_t I3C_ReadSensor(uint8_t deviceAddress, uint32_t regAddress, uint8_t *regData, size_t dataSize)
{
    status_t result                  = kStatus_Success;
    i3c_master_transfer_t masterXfer = {0};
    uint32_t timeout                 = 0U;

    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.direction      = kI3C_Read;
    masterXfer.busType        = kI3C_TypeI3CSdr;
    masterXfer.subaddress     = regAddress;
    masterXfer.subaddressSize = 1;
    masterXfer.data           = regData;
    masterXfer.dataSize       = dataSize;
    masterXfer.flags          = kI3C_TransferDefaultFlag;

    g_masterCompletionFlag = false;
    g_completionStatus     = kStatus_Success;
    result                 = I3C_MasterTransferNonBlocking(EXAMPLE_MASTER, &g_i3c_m_handle, &masterXfer);
    if (kStatus_Success != result)
    {
        return result;
    }

    while (!g_masterCompletionFlag)
    {
        timeout++;
        if ((g_completionStatus != kStatus_Success) || (timeout > I3C_TIME_OUT_INDEX))
        {
            break;
        }
    }

    if (timeout == I3C_TIME_OUT_INDEX)
    {
        result = kStatus_Timeout;
    }
    result = g_completionStatus;

    return result;
}

status_t p3t1755_set_dynamic_address(void)
{
    status_t result                  = kStatus_Success;
    i3c_master_transfer_t masterXfer = {0};
    uint8_t g_master_txBuff[1];

    /* Reset dynamic address. */
    g_master_txBuff[0]      = CCC_RSTDAA;
    masterXfer.slaveAddress = 0x7E;
    masterXfer.data         = g_master_txBuff;
    masterXfer.dataSize     = 1;
    masterXfer.direction    = kI3C_Write;
    masterXfer.busType      = kI3C_TypeI3CSdr;
    masterXfer.flags        = kI3C_TransferDefaultFlag;
    result                  = I3C_MasterTransferBlocking(EXAMPLE_MASTER, &masterXfer);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Assign dynmic address. */
    memset(&masterXfer, 0, sizeof(masterXfer));
    g_master_txBuff[0]      = CCC_SETDASA;
    masterXfer.slaveAddress = 0x7E;
    masterXfer.data         = g_master_txBuff;
    masterXfer.dataSize     = 1;
    masterXfer.direction    = kI3C_Write;
    masterXfer.busType      = kI3C_TypeI3CSdr;
    masterXfer.flags        = kI3C_TransferNoStopFlag;
    result                  = I3C_MasterTransferBlocking(EXAMPLE_MASTER, &masterXfer);
    if (result != kStatus_Success)
    {
        return result;
    }

    memset(&masterXfer, 0, sizeof(masterXfer));
    g_master_txBuff[0]      = SENSOR_ADDR << 1;
    masterXfer.slaveAddress = SENSOR_SLAVE_ADDR;
    masterXfer.data         = g_master_txBuff;
    masterXfer.dataSize     = 1;
    masterXfer.direction    = kI3C_Write;
    masterXfer.busType      = kI3C_TypeI3CSdr;
    masterXfer.flags        = kI3C_TransferDefaultFlag;
    return I3C_MasterTransferBlocking(EXAMPLE_MASTER, &masterXfer);
}

/*!
 * @brief Main function
 */
int main(void)
{
    i3c_master_config_t masterConfig;
    p3t1755_config_t p3t1755Config;

    /* Attach peripheral clock */
    CLOCK_SetClockDiv(kCLOCK_DivI3C0_FCLK, 4U);
    CLOCK_AttachClk(kFRO_HF_DIV_to_I3C0FCLK);

    ctimer_config_t config;
    /* Init hardware*/
    CLOCK_SetClockDiv(kCLOCK_DivCTIMER1, 1u);
    CLOCK_AttachClk(kFRO_HF_to_CTIMER1);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    I3C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Hz.i2cBaud          = EXAMPLE_I2C_BAUDRATE;
    masterConfig.baudRate_Hz.i3cPushPullBaud  = EXAMPLE_I3C_PP_BAUDRATE;
    masterConfig.baudRate_Hz.i3cOpenDrainBaud = EXAMPLE_I3C_OD_BAUDRATE;
    masterConfig.enableOpenDrainStop          = false;
    masterConfig.disableTimeout               = true;
    I3C_MasterInit(EXAMPLE_MASTER, &masterConfig, I3C_MASTER_CLOCK_FREQUENCY);

    /* Create I3C handle. */
    I3C_MasterTransferCreateHandle(EXAMPLE_MASTER, &g_i3c_m_handle, &masterCallback, NULL);

    status_t result = p3t1755_set_dynamic_address();
    if (result != kStatus_Success)
    {
        PRINTF("\r\nP3T1755 set dynamic address failed.\r\n");
    }

    p3t1755Config.writeTransfer = I3C_WriteSensor;
    p3t1755Config.readTransfer  = I3C_ReadSensor;
    p3t1755Config.sensorAddress = SENSOR_ADDR;
    P3T1755_Init(&p3t1755Handle, &p3t1755Config);

    CTIMER_GetDefaultConfig(&config);

    CTIMER_Init(CTIMER, &config);

    /* Configuration 0 */
    matchConfig0.enableCounterReset = true;
    matchConfig0.enableCounterStop  = false;
    matchConfig0.matchValue         = CTIMER_CLK_FREQ >> 10;
    matchConfig0.outControl         = kCTIMER_Output_Toggle;
    matchConfig0.outPinInitState    = false;
    matchConfig0.enableInterrupt    = true;

    /* Create different ctimer_callback_table array for different CTimer instance. */
	CTIMER_RegisterCallBack(CTIMER, &ctimer_callback_table[0], kCTIMER_MultipleCallback);

	/*
	 * Macros CTIMER_MAT0_OUT and CTIMER_MAT1_OUT are nominal match output, instead of
	 * hardware MR0 and MR1 register match output.
	 * So CTIMER_MAT0_OUT can be defined as kCTIMER_Match_1, CTIMER_MAT1_OUT can be defined
	 * as kCTIMER_Match_3, which means they are MR1 and MR3 register match output.
	 */
	CTIMER_SetupMatch(CTIMER, CTIMER_MAT0_OUT, &matchConfig0);
	CTIMER_StartTimer(CTIMER);

    uint32_t input = 0;
    char input_buf[16] = {0}, ch = '\0';
    uint8_t input_idx = 0;

    double temperature;
    while ((ch != '\r') && (ch != '\n'))
    {
	    if (P3T1755_ReadTemperature(&p3t1755Handle, &temperature) != kStatus_Success)
		{
			PRINTF("Read P3T1755 Error\r\n");
		}
	    else
	    {
	        factor = (uint16_t)(temperature * 16 + 40);
	    }
        ch = GETCHAR();
        PUTCHAR(ch);
        input_buf[input_idx++] = ch;
    }

    if (strlen(input_buf) > 0)
    {
    	input = atoi(input_buf);
    }

    if (input)
    {
    	PRINTF("\r\n%u\r\n", random % input);
    }
    else
    {
    	uint32_t result = random % (uint64_t)10000;
    	PRINTF("%u.%02u\r\n", result / 100, result % 100);
    }

    while (1)
    {
        result = P3T1755_ReadTemperature(&p3t1755Handle, &temperature);
    	if (result != kStatus_Success)
    	{
    	    PRINTF("\r\nP3T1755 read temperature failed.\r\n");
    	}
    	else
    	{
            PRINTF("Temp:%.4f", temperature);
            PRINTF("\b\b\b\b\b\b\b\b\b\b\b\b");
    	}
        SDK_DelayAtLeastUs(1000000, CLOCK_GetCoreSysClkFreq());
    }
}
