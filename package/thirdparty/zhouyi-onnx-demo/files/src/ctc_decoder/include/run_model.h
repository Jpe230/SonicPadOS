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


#ifndef RNN_T_RUN_MODEL_H_
#define RNN_T_RUN_MODEL_H_

#include <vector>
#include <memory>
#include "oruntime_types.h"
#include "oruntime_api.h"
#include "oruntime_quantize.h"


typedef struct {
    void* data;
    size_t len;
} INPUT_BUF;


std::vector<std::shared_ptr<oruntime::ORTTensor>> run_model(std::string& model_file,
                                                            std::vector<std::string>& input_files,
                                                            std::string& ouput_dir);

std::vector<std::shared_ptr<oruntime::ORTTensor>> run_model_buf(std::string& model_file,
                                                            std::vector<INPUT_BUF>& input_bufs,
                                                            std::string& ouput_dir);


#endif  // RNN_T_RUN_MODEL_H_
