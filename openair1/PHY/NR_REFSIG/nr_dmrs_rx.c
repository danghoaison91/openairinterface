/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.0  (the "License"); you may not use this file
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

/*! \file PHY/NR_REFSIG/nr_dl_dmrs.c
* \brief Top-level routines for generating DMRS from 38-211
* \author
* \date 2018
* \version 0.1
* \company Eurecom
* \email:
* \note
* \warning
*/

//#define DEBUG_DL_DMRS
//#define NR_PBCH_DMRS_LENGTH_DWORD 5
//#define NR_PBCH_DMRS_LENGTH 144

#ifdef USER_MODE
#include <stdio.h>
#include <stdlib.h>
#endif

#include "refsig_defs_ue.h"
#include "PHY/defs_nr_UE.h"
#include "log.h"

/*Table 7.4.1.1.2-1/2 from 38.211 */
int wf1[8][2] = {{1,1},{1,-1},{1,1},{1,-1},{1,1},{1,-1},{1,1},{1,1}};
int wt1[8][2] = {{1,1},{1,1},{1,1},{1,1},{1,-1},{1,-1},{1,-1},{1,-1}};
int wf2[12][2] = {{1,1},{1,-1},{1,1},{1,-1},{1,1},{1,-1},{1,1},{1,1},{1,1},{1,-1},{1,1},{1,1}};
int wt2[12][2] = {{1,1},{1,1},{1,1},{1,1},{1,1},{1,1},{1,-1},{1,-1},{1,-1},{1,-1},{1,-1},{1,-1}};

//short nr_mod_table[14] = {0,0,-23170,-23170,23170,23170,-23170,-23170,-23170,23170,23170,-23170,23170,23170};
  short nr_mod_table[14] = {0,0,23170,-23170,-23170,23170,23170,-23170,23170,23170,-23170,-23170,-23170,23170};
//short nr_mod_table[14] = {0,0,23170,23170,-23170,-23170,23170,23170,23170,-23170,-23170,23170,-23170,-23170};
//extern short nr_mod_table[NR_MOD_TABLE_SIZE_SHORT];


int nr_pdsch_dmrs_rx(PHY_VARS_NR_UE *ue,
						unsigned int ncp,
						unsigned int Ns,
						unsigned int nr_gold_pdsch[2][20][2][21],
						int32_t *output,
						unsigned short p,
						int length_dmrs,
						unsigned short nb_pdsch_rb)
{
  int32_t qpsk[4],nqpsk[4],*qpsk_p, n;
  int w,mprime,ind,l,ind_dword,ind_qpsk_symb,kp,lp, config_type, k;
  short pamp;

  typedef int array_of_w[2];
  array_of_w *wf;
  array_of_w *wt;

  config_type = 1;
  printf("dmrs config type %d port %d\n", config_type, p);

  // Compute the correct pilot amplitude, sqrt_rho_b = Q3.13
  pamp = 23170; //ONE_OVER_SQRT2_Q15;

  // This includes complex conjugate for channel estimation
  ((short *)&qpsk[0])[0] = pamp;
  ((short *)&qpsk[0])[1] = -pamp;
  ((short *)&qpsk[1])[0] = -pamp;
  ((short *)&qpsk[1])[1] = -pamp;
  ((short *)&qpsk[2])[0] = pamp;
  ((short *)&qpsk[2])[1] = pamp;
  ((short *)&qpsk[3])[0] = -pamp;
  ((short *)&qpsk[3])[1] = pamp;

  ((short *)&nqpsk[0])[0] = -pamp;
  ((short *)&nqpsk[0])[1] = pamp;
  ((short *)&nqpsk[1])[0] = pamp;
  ((short *)&nqpsk[1])[1] = pamp;
  ((short *)&nqpsk[2])[0] = -pamp;
  ((short *)&nqpsk[2])[1] = -pamp;
  ((short *)&nqpsk[3])[0] = pamp;
  ((short *)&nqpsk[3])[1] = -pamp;

  wf = (config_type==0) ? wf1 : wf2;
  wt = (config_type==0) ? wt1 : wt2;

  if (config_type > 1)
      LOG_E(PHY,"Bad PDSCH DMRS config type %d\n", config_type);

  if ((p>=1000) && (p<((config_type==0) ? 1008 : 1012))) {
      if (/*ue->frame_parms.Ncp == NORMAL*/ncp ==0) {

        // r_n from 38.211 7.4.1.1
        for (n=0; n<nb_pdsch_rb*((config_type==0) ? 3:2); n++) {
        	for (lp =0; lp<length_dmrs; lp++){
        		for (kp=0; kp<2; kp++){
        			w = (wf[p-1000][kp])*(wt[p-1000][lp]);
        			qpsk_p = (w==1) ? qpsk : nqpsk;

        			ind = 2*n+kp;
        			ind_dword = ind>>4;
        			ind_qpsk_symb = ind&0xf;

        			output[k] = qpsk_p[(ue->nr_gold_pdsch[0][Ns][lp][ind_dword]>>(2*ind_qpsk_symb))&3];


#ifdef DEBUG_DL_DMRS
          LOG_I(PHY,"Ns %d, p %d, ind_dword %d, ind_qpsk_symbol %d\n",
                Ns,p,idx_dword,idx_qpsk_symb);
          LOG_I(PHY,"index = %d\n",(nr_gold_pdsch[0][Ns][lprime][ind_dword]>>(2*ind_qpsk_symb))&3);
#endif

          	  	  	k++;
        		}
        	}
        }
      } else {
        LOG_E(PHY,"extended cp not supported for PDSCH DMRS yet\n");
      }
  } else {
    LOG_E(PHY,"Illegal p %d PDSCH DMRS port\n",p);
  }

  return(0);
}

int nr_pbch_dmrs_rx(unsigned int *nr_gold_pbch,
					int32_t *output	)
{
	int m;

	/// BPSK modulation
    for (m=0; m<NR_PBCH_DMRS_LENGTH; m++) {
      ((int16_t*)output)[m<<1] = nr_mod_table[((1 + ((nr_gold_pbch[m>>5]&(1<<(m&0x1f)))>>(m&0x1f)))<<1)];
      ((int16_t*)output)[(m<<1)+1] = nr_mod_table[((1 + ((nr_gold_pbch[m>>5]&(1<<(m&0x1f)))>>(m&0x1f)))<<1) + 1];
#ifdef DEBUG_PBCH
	//printf("nr_gold_pbch[m>>5] %x\n",nr_gold_pbch[m>>5]);
       if (m<16)
	printf("m %d  output %d %d addr %p\n", m, ((int16_t*)output)[m<<1], ((int16_t*)output)[(m<<1)+1],&output[0]);
#endif
    }

  return(0);
}

