/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/*! \file PHY/LTE_TRANSPORT/dci_tools_common_extern.h
 * \brief PHY Support routines (eNB/UE) for filling PDSCH/PUSCH/DLSCH/ULSCH data structures based on DCI PDUs generated by eNB MAC scheduler.
 * \author R. Knopp
 * \date 2011
 * \version 0.1
 * \company Eurecom
 * \email: knopp@eurecom.fr
 * \note
 * \warning
 */
#include <stdint.h>

extern uint32_t localRIV2alloc_LUT6[32];
extern uint32_t distRIV2alloc_even_LUT6[32];
extern uint32_t distRIV2alloc_odd_LUT6[32];
extern uint16_t RIV2nb_rb_LUT6[32];
extern uint16_t RIV2first_rb_LUT6[32];
extern uint16_t RIV_max6;

extern uint32_t localRIV2alloc_LUT25[512];
extern uint32_t distRIV2alloc_even_LUT25[512];
extern uint32_t distRIV2alloc_odd_LUT25[512];
extern uint16_t RIV2nb_rb_LUT25[512];
extern uint16_t RIV2first_rb_LUT25[512];
extern uint16_t RIV_max25;


extern uint32_t localRIV2alloc_LUT50_0[1600];
extern uint32_t localRIV2alloc_LUT50_1[1600];
extern uint32_t distRIV2alloc_gap0_even_LUT50_0[1600];
extern uint32_t distRIV2alloc_gap0_odd_LUT50_0[1600];
extern uint32_t distRIV2alloc_gap0_even_LUT50_1[1600];
extern uint32_t distRIV2alloc_gap0_odd_LUT50_1[1600];
extern uint32_t distRIV2alloc_gap1_even_LUT50_0[1600];
extern uint32_t distRIV2alloc_gap1_odd_LUT50_0[1600];
extern uint32_t distRIV2alloc_gap1_even_LUT50_1[1600];
extern uint32_t distRIV2alloc_gap1_odd_LUT50_1[1600];
extern uint16_t RIV2nb_rb_LUT50[1600];
extern uint16_t RIV2first_rb_LUT50[1600];
extern uint16_t RIV_max50;

extern uint32_t localRIV2alloc_LUT100_0[6000];
extern uint32_t localRIV2alloc_LUT100_1[6000];
extern uint32_t localRIV2alloc_LUT100_2[6000];
extern uint32_t localRIV2alloc_LUT100_3[6000];
extern uint32_t distRIV2alloc_gap0_even_LUT100_0[6000];
extern uint32_t distRIV2alloc_gap0_odd_LUT100_0[6000];
extern uint32_t distRIV2alloc_gap0_even_LUT100_1[6000];
extern uint32_t distRIV2alloc_gap0_odd_LUT100_1[6000];
extern uint32_t distRIV2alloc_gap0_even_LUT100_2[6000];
extern uint32_t distRIV2alloc_gap0_odd_LUT100_2[6000];
extern uint32_t distRIV2alloc_gap0_even_LUT100_3[6000];
extern uint32_t distRIV2alloc_gap0_odd_LUT100_3[6000];
extern uint32_t distRIV2alloc_gap1_even_LUT100_0[6000];
extern uint32_t distRIV2alloc_gap1_odd_LUT100_0[6000];
extern uint32_t distRIV2alloc_gap1_even_LUT100_1[6000];
extern uint32_t distRIV2alloc_gap1_odd_LUT100_1[6000];
extern uint32_t distRIV2alloc_gap1_even_LUT100_2[6000];
extern uint32_t distRIV2alloc_gap1_odd_LUT100_2[6000];
extern uint32_t distRIV2alloc_gap1_even_LUT100_3[6000];
extern uint32_t distRIV2alloc_gap1_odd_LUT100_3[6000];
extern uint16_t RIV2nb_rb_LUT100[6000];
extern uint16_t RIV2first_rb_LUT100[6000];
extern uint16_t RIV_max100;

extern int8_t *delta_PUCCH_lut;
extern int8_t delta_PUSCH_acc[4];
extern int8_t delta_PUSCH_abs[4];