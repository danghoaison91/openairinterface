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

/*! \file l2_interface.c
 * \brief layer 2 interface, used to support different RRC sublayer
 * \author Raymond Knopp and Navid Nikaein
 * \date 2010-2014
 * \version 1.0
 * \company Eurecom
 * \email: raymond.knopp@eurecom.fr
 */

#include "rrc_defs.h"
#include "rrc_proto.h"

typedef uint32_t channel_t;

int8_t
nr_mac_rrc_data_ind_ue(
    const module_id_t     module_id,
    const int             CC_id,
    const uint8_t         gNB_index,
    const uint32_t*       frameP,
    const channel_t       channel,
    const uint8_t*        pduP,
    const sdu_size_t      pdu_len){

    switch(channel){
        case NR_BCCH_BCH:
            nr_rrc_ue_decode_NR_BCCH_BCH_Message( module_id, gNB_index, (uint8_t*)pduP, pdu_len);
            break;
        default:
            break;
    }


    return(0);

}