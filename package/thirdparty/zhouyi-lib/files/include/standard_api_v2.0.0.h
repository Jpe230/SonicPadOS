/**********************************************************************************
 * This file is CONFIDENTIAL and any use by you is subject to the terms of the
 * agreement between you and Arm China or the terms of the agreement between you
 * and the party authorised by Arm China to disclose this file to you.
 * The confidential and proprietary information contained in this file
 * may only be used by a person authorised under and to the extent permitted
 * by a subsisting licensing agreement from Arm China.
 *
 *        (C) Copyright 2020 Arm Technology (China) Co. Ltd.
 *                    All rights reserved.
 *
 * This entire notice must be reproduced on all copies of this file and copies of
 * this file may only be made by a person if such person is permitted to do so
 * under the terms of a subsisting license agreement from Arm China.
 *
 *********************************************************************************/

/**
 * @file  standard_api_v2.0.0.h
 * @brief AIPU User Mode Driver (UMD) Standard API header
 * @version 2.0.0
 */

#ifndef _STANDARD_API_V_2_0_0_H_
#define _STANDARD_API_V_2_0_0_H_

#include <stdint.h>

typedef struct ctx_handle aipu_ctx_handle_t;

/**
 * @brief AIPU tensor descriptions:
 *        format: layout/shape/data type
 *        buffer: address info
 */

typedef enum {
    TENSOR_LAYOUT_NONE = 0x0,
    TENSOR_LAYOUT_NHWC,
    TENSOR_LAYOUT_NCHW,
    TENSOR_LAYOUT_NWH,
    TENSOR_LAYOUT_NC
} aipu_tensor_layout_t;

typedef struct tensor_shape {
    uint32_t N;
    uint32_t H;
    uint32_t W;
    uint32_t C;
} aipu_tensor_shape_t;

typedef enum {
    TENSOR_DATA_TYPE_NONE = 0x0,
    TENSOR_DATA_TYPE_U8 = 0x2,
    TENSOR_DATA_TYPE_S8 = 0x3,
    TENSOR_DATA_TYPE_U16 = 0x4,
    TENSOR_DATA_TYPE_S16 = 0x5,
} aipu_data_type_t;

typedef struct aipu_tensor_fmt {
    aipu_tensor_layout_t layout;
    aipu_tensor_shape_t  shape;
    aipu_data_type_t     data_type;
} aipu_tensor_fmt_t;

typedef struct tensor_desc {
    uint32_t id;
    uint32_t size;
    aipu_tensor_fmt_t fmt;
} aipu_tensor_desc_t;

typedef struct buffer {
    uint32_t id;
    void*    va;
    uint32_t size;
} aipu_buffer_t;

/**
 * @brief AIPU tensor descriptions struct of specific types: input/output/dump
 *        tensor_info:   basic format info.
 *        tensor_buffer: buffer address info.
 */
typedef struct tensor_info {
    uint32_t number;
    const aipu_tensor_desc_t* desc;
} aipu_tensor_info_t;

typedef struct tensor_buffer {
    uint32_t number;
    const aipu_buffer_t* tensors;
} aipu_tensor_buffer_t;

/**
 * @brief AIPU loadable graph descriptions:
 *        graph_desc:   graph basic info. returned by AIPU_load_graph
 *        buffer_alloc: graph buffers info. returned by AIPU_alloc_tensor_buffers
 */
typedef struct graph_desc {
    uint32_t id;                  /**< graph ID */
    uint32_t graph_version;       /**< graph binary version number */
    uint8_t  building_tool_major; /**< major number of the build_tool generating this binary */
    uint8_t  building_tool_minor; /**< minor number of the build_tool generating this binary */
    uint16_t build_version;       /**< version number of the build_tool generating this binary */
    aipu_tensor_info_t inputs;    /**< struct contains input tensor descriptors info. */
    aipu_tensor_info_t outputs;   /**< struct contains output tensor descriptors info. */
    aipu_tensor_info_t dumps;     /**< struct contains dump tensor descriptors info. */
} aipu_graph_desc_t;

typedef struct aipu_buffer_alloc_info {
    uint32_t handle;              /**< buffer handle */
    aipu_tensor_buffer_t inputs;  /**< struct contains info. of input tensor buffers allocated */
    aipu_tensor_buffer_t outputs; /**< struct contains info. of output tensor buffers allocated */
    aipu_tensor_buffer_t dumps;   /**< struct contains info. of dump tensor buffers allocated */
} aipu_buffer_alloc_info_t;

/**
 * @brief AIPU simulation configuration; used only on x86-linux simulation platform.
 */
typedef struct simulation_config {
    char* simulator;      /**< simulator executable path */
    char* cfg_file_dir;   /**< simulator runtime config file path */
    char* output_dir;     /**< simulator dump path */
} aipu_simulation_config_t;

/**
 * @brief AIPU job status; returned by status querying API AIPU_get_job_end_status().
 */
typedef enum {
    AIPU_JOB_STATUS_NO_STATUS, /**< no status */
    AIPU_JOB_STATUS_DONE,      /**< job execution successfully */
    AIPU_JOB_STATUS_EXCEPTION  /**< job execution failed, encountering exception */
} aipu_job_status_t;

/**
 * @brief AIPU debug info struct; returned by UMD API for AIPU debugger to use
 */
typedef struct aipu_debug_info {
    uint32_t instruction_base_pa; /**< instruction section base address (physical) */
    uint32_t start_pc_pa;         /**< start PC pointer address (physical) */
    uint32_t interrupt_pc_pa;     /**< interrupt handler base address (physical) */
} aipu_debug_info_t;

/**
 * @brief AIPU memory dump flag; set by UMD application via API AIPU_set_dump_options()
 */
typedef enum {
    AIPU_DUMP_INST = 1 << 0,          /**< dump instruction section(s) */
    AIPU_DUMP_RO = 1 << 1,            /**< dump read-only data section(s) */
    AIPU_DUMP_STACK = 1 << 2,         /**< dump AIPU stack section(s) */
    AIPU_DUMP_STATIC_TENSOR = 1 << 3, /**< dump static data section(s) */
    AIPU_DUMP_REUSE_TENSOR = 1 << 4,  /**< dump reuse data section(s) */
    AIPU_DUMP_OUT_TENSOR = 1 << 5,    /**< dump output data tensor(s) */
    AIPU_DUMP_INTER_TENSOR = 1 << 6,  /**< dump intermediate tensor(s) */
    AIPU_DUMP_BEFORE_RUN = 1 << 7,    /**< dump after loading & before job execution */
    AIPU_DUMP_AFTER_RUN = 1 << 8,     /**< dump after AIPU job execution done */
    AIPU_DUMP_MAX = 1 << 9            /**< dump flag max. value, invalid */
} aipu_dump_flag_t;

/**
 * @brief This aipu_status_t enumeration captures the result of any API function
 *        that has been executed. Success is represented by AIPU_STATUS_SUCCESS
 *        which has a value of zero. Error statuses are assigned positive integers
 *        and their identifiers start with the AIPU_STATUS_ERROR prefix.
 */
typedef enum {
    AIPU_STATUS_SUCCESS                    = 0x0,
    AIPU_STATUS_ERROR_NULL_PTR             = 0x1,
    AIPU_STATUS_ERROR_INVALID_CTX          = 0x2,
    AIPU_STATUS_ERROR_OPEN_DEV_FAIL        = 0x3,
    AIPU_STATUS_ERROR_DEV_ABNORMAL         = 0x4,
    AIPU_STATUS_ERROR_DEINIT_FAIL          = 0x5,
    AIPU_STATUS_ERROR_INVALID_CONFIG       = 0x6,
    AIPU_STATUS_ERROR_GVERSION_UNSUPPORTED = 0x7,
    AIPU_STATUS_ERROR_TARGET_NOT_FOUND     = 0x8,
    AIPU_STATUS_ERROR_INVALID_GBIN         = 0x9,
    AIPU_STATUS_ERROR_GRAPH_NOT_EXIST      = 0xA,
    AIPU_STATUS_ERROR_OPEN_FILE_FAIL       = 0xB,
    AIPU_STATUS_ERROR_MAP_FILE_FAIL        = 0xC,
    AIPU_STATUS_ERROR_READ_FILE_FAIL       = 0xD,
    AIPU_STATUS_ERROR_WRITE_FILE_FAIL      = 0xE,
    AIPU_STATUS_ERROR_JOB_NOT_EXIST        = 0xF,
    AIPU_STATUS_ERROR_JOB_NOT_SCHED        = 0x10,
    AIPU_STATUS_ERROR_JOB_SCHED            = 0x11,
    AIPU_STATUS_ERROR_JOB_NOT_END          = 0x12,
    AIPU_STATUS_ERROR_JOB_EXCEPTION        = 0x13,
    AIPU_STATUS_ERROR_JOB_TIMEOUT          = 0x14,
    AIPU_STATUS_ERROR_INVALID_OPTIONS      = 0x15,
    AIPU_STATUS_ERROR_INVALID_PATH         = 0x16,
    AIPU_STATUS_ERROR_OP_NOT_SUPPORTED     = 0x17,
    AIPU_STATUS_ERROR_INVALID_OP           = 0x18,
    AIPU_STATUS_ERROR_INVALID_SIZE         = 0x19,
    AIPU_STATUS_ERROR_INVALID_HANDLE       = 0x1A,
    AIPU_STATUS_ERROR_BUSY_HANDLE          = 0x1B,
    AIPU_STATUS_ERROR_BUF_ALLOC_FAIL       = 0x1C,
    AIPU_STATUS_ERROR_BUF_FREE_FAIL        = 0x1D,
    AIPU_STATUS_MAX = 0x1E
} aipu_status_t;

/**
 * @brief This API is used to query additional information about a status returned by UMD API
 *
 * @param status Status returned by UMD standard API
 * @param msg    Pointer to a memory location allocated by application where UMD stores the
 *               message string pointer
 *
 * @retval AIPU_STATUS_SUCCESS
 * @retval AIPU_STATUS_ERROR_NULL_PTR
 */
aipu_status_t AIPU_get_status_msg(aipu_status_t status, const char** msg);
/**
 * @brief This API is used to initialize AIPU UMD context
 *
 * @param[out] ctx Pointer to a memory location allocated by application where UMD stores the
 *                 opaque context handle struct
 *
 * @retval AIPU_STATUS_SUCCESS
 * @retval AIPU_STATUS_ERROR_NULL_PTR
 * @retval AIPU_STATUS_ERROR_OPEN_DEV_FAIL
 * @retval AIPU_STATUS_ERROR_DEV_ABNORMAL
 */
aipu_status_t AIPU_init_ctx(aipu_ctx_handle_t** ctx);
/**
 * @brief This API is used to destroy AIPU UMD context
 *
 * @param[in] ctx Pointer to a context handle struct returned by AIPU_init_ctx
 *
 * @retval AIPU_STATUS_SUCCESS
 * @retval AIPU_STATUS_ERROR_NULL_PTR
 * @retval AIPU_STATUS_ERROR_DEINIT_FAIL
 */
aipu_status_t AIPU_deinit_ctx(const aipu_ctx_handle_t* ctx);
/**
 * @brief This API is used to configure AIPU simulation platform
 *
 * @param[in] ctx    Pointer to a context handle struct returned by AIPU_init_ctx
 * @param[in] config Pointer to a memory location allocated by application where stores the
 *                   configurations
 *
 * @retval AIPU_STATUS_SUCCESS
 * @retval AIPU_STATUS_ERROR_NULL_PTR
 * @retval AIPU_STATUS_ERROR_INVALID_CTX
 * @retval AIPU_STATUS_ERROR_INVALID_CONFIG
 * @retval AIPU_STATUS_ERROR_OP_NOT_SUPPORTED
 *
 * @note works only for x86-linux simulation platform
 */
aipu_status_t AIPU_config_simulation(const aipu_ctx_handle_t* ctx,
    const aipu_simulation_config_t* config);
/**
 * @brief This API is used to load a graph binary for driver to parse and alloc static buffers
 *
 * @param[in]  ctx   Pointer to a context handle struct returned by AIPU_init_ctx
 * @param[in]  garph Pointer to a memory location allocated by application where stores the graph
 * @param[in]  size  Graph buffer size
 * @param[out] gdesc Pointer to a memory location allocated by application where UMD stores the
 *                   graph descriptor
 *
 * @retval AIPU_STATUS_SUCCESS
 * @retval AIPU_STATUS_ERROR_NULL_PTR
 * @retval AIPU_STATUS_ERROR_INVALID_CTX
 * @retval AIPU_STATUS_ERROR_GVERSION_UNSUPPORTED
 * @retval AIPU_STATUS_ERROR_TARGET_NOT_FOUND
 * @retval AIPU_STATUS_ERROR_INVALID_GBIN
 */
aipu_status_t AIPU_load_graph(const aipu_ctx_handle_t* ctx,
    const void* graph, uint32_t size, aipu_graph_desc_t* gdesc);
/**
 * @brief This API is a wrapper of AIPU_load_graph which loads an offline built graph binary
 *        file from file system.
 *
 * @param[in]  ctx        Pointer to a context handle struct returned by AIPU_init_ctx
 * @param[in]  garph_file Loadable graph binary file path
 * @param[out] gdesc      Pointer to a memory location allocated by application where UMD stores
 *                        the graph descriptor
 *
 * @retval AIPU_STATUS_SUCCESS
 * @retval AIPU_STATUS_ERROR_OPEN_GBIN_FAIL
 * @retval AIPU_STATUS_ERROR_MAP_GBIN_FAIL
 * @retval Other values returned by AIPU_load_graph
 */
aipu_status_t AIPU_load_graph_helper(const aipu_ctx_handle_t* ctx,
    const char* graph_file, aipu_graph_desc_t* gdesc);
/**
 * @brief This API is used to unload a loaded graph
 *
 * @param[in] ctx   Pointer to a context handle struct returned by AIPU_init_ctx
 * @param[in] gdesc Pointer to a graph descriptor returned by AIPU_load_graph
 *
 * @retval AIPU_STATUS_SUCCESS
 * @retval AIPU_STATUS_ERROR_INVALID_CTX
 * @retval AIPU_STATUS_ERROR_GRAPH_NOT_EXIST
 */
aipu_status_t AIPU_unload_graph(const aipu_ctx_handle_t* ctx, const aipu_graph_desc_t* gdesc);
/**
 * @brief This API is used to allocate buffers for all input and output and dump (if any) tensors
 *        of a graph.
 *
 * @param[in]  ctx   Pointer to a context handle struct returned by AIPU_init_ctx
 * @param[in]  gdesc Pointer to a graph descriptor returned by AIPU_load_graph
 * @param[out] info  Pointer to a memory location allocated by application where UMD stores
 *                   the buffer info struct
 *
 * @retval AIPU_STATUS_SUCCESS
 * @retval AIPU_STATUS_ERROR_NULL_PTR
 * @retval AIPU_STATUS_ERROR_INVALID_CTX
 * @retval AIPU_STATUS_ERROR_GRAPH_NOT_EXIST
 * @retval AIPU_STATUS_ERROR_BUF_ALLOC_FAIL
 */
aipu_status_t AIPU_alloc_tensor_buffers(const aipu_ctx_handle_t* ctx, const aipu_graph_desc_t* gdesc,
    aipu_buffer_alloc_info_t* info);
/**
 * @brief This API is used to free buffers allocated by AIPU_alloc_tensor_buffers
 *
 * @param[in] ctx    Pointer to a context handle struct returned by AIPU_init_ctx
 * @param[in] handle Buffer handle returned by AIPU_alloc_tensor_buffers
 *
 * @retval AIPU_STATUS_SUCCESS
 * @retval AIPU_STATUS_ERROR_INVALID_CTX
 * @retval AIPU_STATUS_ERROR_INVALID_HANDLE
 */
aipu_status_t AIPU_free_tensor_buffers(const aipu_ctx_handle_t* ctx, uint32_t handle);
/**
 * @brief This API is used to create a new job for a graph with provided buffer handle.
 *
 * @param[in]  ctx        Pointer to a context handle struct returned by AIPU_init_ctx
 * @param[in]  gdesc      Pointer to a graph descriptor returned by AIPU_load_graph
 * @param[in]  buf_handle Buffer handle returned by AIPU_alloc_tensor_buffers
 * @param[out] job_id     Pointer to a memory location allocated by application where UMD stores
 *                        the new created job ID
 *
 * @retval AIPU_STATUS_SUCCESS
 * @retval AIPU_STATUS_ERROR_NULL_PTR
 * @retval AIPU_STATUS_ERROR_INVALID_CTX
 * @retval AIPU_STATUS_ERROR_GRAPH_NOT_EXIST
 * @retval AIPU_STATUS_ERROR_INVALID_HANDLE
 */
aipu_status_t AIPU_create_job(const aipu_ctx_handle_t* ctx, const aipu_graph_desc_t* gdesc,
    uint32_t buf_handle, uint32_t* job_id);
/**
 * @brief This API is used to flush a new computation job onto AIPU
 *
 * @param[in] ctx Pointer to a context handle struct returned by AIPU_init_ctx
 * @param[in] id  Job ID returned by AIPU_create_job
 *
 * @retval AIPU_STATUS_SUCCESS
 * @retval AIPU_STATUS_ERROR_NULL_PTR
 * @retval AIPU_STATUS_ERROR_INVALID_CTX
 * @retval AIPU_STATUS_ERROR_JOB_NOT_EXIST
 */
aipu_status_t AIPU_flush_job(const aipu_ctx_handle_t* ctx, uint32_t id);
/**
 * @brief This API is used to flush a new computation job onto AIPU
 *
 * @param[in] ctx      Pointer to a context handle struct returned by AIPU_init_ctx
 * @param[in] id       Job ID returned by AIPU_create_job
 * @param[in] time_out Time out (in millisecond) specified by application for this job
 *                     (= -1 means no time out limiting)
 *
 * @retval AIPU_STATUS_SUCCESS
 * @retval AIPU_STATUS_ERROR_JOB_EXCEPTION
 * @retval AIPU_STATUS_ERROR_JOB_TIMEOUT
 * @retval Other values returned by AIPU_flush_job
 */
aipu_status_t AIPU_finish_job(const aipu_ctx_handle_t* ctx, uint32_t id, int32_t time_out);
/**
 * @brief This API is used to get the execution status of a job scheduled via AIPU_flush_job.
 *
 * @param[in]  ctx      Pointer to a context handle struct returned by AIPU_init_ctx
 * @param[in]  id       Job ID returned by AIPU_create_job
 * @param[in]  time_out Time out (in millisecond) specified by application for this job
 *                      (= -1 means no time out limiting)
 * @param[out] status   Pointer to a memory location allocated by application where UMD stores
 *                      the job status
 *
 * @retval AIPU_STATUS_SUCCESS
 * @retval AIPU_STATUS_ERROR_NULL_PTR
 * @retval AIPU_STATUS_ERROR_INVALID_CTX
 * @retval AIPU_STATUS_ERROR_JOB_NOT_EXIST
 * @retval AIPU_STATUS_ERROR_JOB_NOT_SCHEDULED
 * @retval AIPU_STATUS_ERROR_JOB_TIMEOUT
 */
aipu_status_t AIPU_get_job_status(const aipu_ctx_handle_t* ctx,
    uint32_t id, int32_t time_out, aipu_job_status_t* status);
/**
 * @brief This API is used to clean a finished job object in UMD
 *
 * @param[in] ctx Pointer to a context handle struct returned by AIPU_init_ctx
 * @param[in] id  Job ID returned by AIPU_create_job
 *
 * @retval AIPU_STATUS_SUCCESS
 * @retval AIPU_STATUS_ERROR_NULL_PTR
 * @retval AIPU_STATUS_ERROR_INVALID_CTX
 * @retval AIPU_STATUS_ERROR_JOB_NOT_EXIST
 * @retval AIPU_STATUS_ERROR_JOB_NOT_SCHEDULED
 */
aipu_status_t AIPU_clean_job(const aipu_ctx_handle_t* ctx, uint32_t id);
/**
 * @brief This API is used to set dump options while debugging
 *
 * @param[in] ctx    Pointer to a context handle struct returned by AIPU_init_ctx
 * @param[in] job_id Job ID returned by AIPU_create_job
 * @param[in] flag   Dump flag option combination
 * @param[in] dir    Directory path where UMD dumps the requested files into
 *
 * @retval AIPU_STATUS_SUCCESS
 * @retval AIPU_STATUS_ERROR_NULL_PTR
 * @retval AIPU_STATUS_ERROR_INVALID_CTX
 * @retval AIPU_STATUS_ERROR_JOB_NOT_EXIST
 * @retval AIPU_STATUS_ERROR_INVALID_OPTIONS
 * @retval AIPU_STATUS_ERROR_INVALID_PATH
 */
aipu_status_t AIPU_set_dump_options(const aipu_ctx_handle_t* ctx,
    uint32_t job_id, uint32_t flag, const char* dir);
/**
 * @brief This API returns AIPU external register values to be set for debugger to use
 *
 * @param[in]  ctx    Pointer to a context handle struct returned by AIPU_init_ctx
 * @param[in]  job_id Job ID returned by AIPU_create_job
 * @param[out] info   Pointer to a memory location allocated by application where UMD stores
 *                    the debugging info
 *
 * @retval AIPU_STATUS_SUCCESS
 * @retval AIPU_STATUS_ERROR_NULL_PTR
 * @retval AIPU_STATUS_ERROR_INVALID_CTX
 * @retval AIPU_STATUS_ERROR_JOB_NOT_EXIST
 */
aipu_status_t AIPU_get_debug_info(const aipu_ctx_handle_t* ctx,
    uint32_t job_id, aipu_debug_info_t* info);
/**
 * @brief this API returns the current value in AIPU status register
 *
 * @param[in]  ctx   Pointer to a context handle struct returned by AIPU_init_ctx
 * @param[out] value Pointer to a memory location allocated by application where UMD stores
 *                   the AIPU external status register value readback
 *
 * @retval AIPU_STATUS_SUCCESS
 * @retval AIPU_STATUS_ERROR_NULL_PTR
 * @retval AIPU_STATUS_ERROR_INVALID_CTX
 * @retval AIPU_STATUS_ERROR_OP_NOT_SUPPORTED
 *
 * @note works only for arm-linux platform
 */
aipu_status_t AIPU_get_dev_status(const aipu_ctx_handle_t* ctx, uint32_t* value);

#endif /* _STANDARD_API_V_2_0_0_H_ */