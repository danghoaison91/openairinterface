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

/**********************************************************************
*
* FILENAME    :  sss_nr.c
*
* MODULE      :  Functions for secundary synchronisation signal
*
* DESCRIPTION :  generation of sss
*                3GPP TS 38.211 7.4.2.3 Secondary synchronisation signal
*
************************************************************************/

#include <stdio.h>
#include <assert.h>
#include <errno.h>

#include "PHY/defs_nr_UE.h"
#include "PHY/MODULATION/modulation_UE.h"

#include "PHY/NR_REFSIG/ss_pbch_nr.h"

#define DEFINE_VARIABLES_SSS_NR_H
#include "PHY/NR_REFSIG/sss_nr.h"
#undef DEFINE_VARIABLES_SSS_NR_H

/*******************************************************************
*
* NAME :         init_context_sss_nr
*
* PARAMETERS :   N_ID_2 : element 2 of physical layer cell identity
*                value : { 0, 1, 2 }
*
* RETURN :       generate binary sss sequence (this is a m-sequence)
*                d_sss is a third dimension array depending on
*                Cell identity elements:
*                - N_ID_1 : value from 0 to 335
*                - N_ID_2 : value from 0 to 2
*
* DESCRIPTION :  3GPP TS 38.211 7.4.2.3 Secundary synchronisation signal
*                Sequence generation
*
*********************************************************************/

#define INITIAL_SSS_NR    (7)

void init_context_sss_nr(int amp)
{
  int16_t x0[LENGTH_SSS_NR];
  int16_t x1[LENGTH_SSS_NR];
  int16_t dss_current;
  int m0, m1;

  const int x0_initial[INITIAL_SSS_NR] = { 1, 0, 0, 0, 0, 0, 0 };
  const int x1_initial[INITIAL_SSS_NR] = { 1, 0, 0, 0, 0, 0, 0 };

  for (int i=0; i < INITIAL_SSS_NR; i++) {
    x0[i] = x0_initial[i];
    x1[i] = x1_initial[i];
  }

  for (int i=0; i < (LENGTH_SSS_NR - INITIAL_SSS_NR); i++) {
    x0[i+7] = (x0[i + 4] + x0[i])%(2);
    x1[i+7] = (x1[i + 1] + x1[i])%(2);
  }

  for (int N_ID_2 = 0; N_ID_2 < N_ID_2_NUMBER; N_ID_2++) {

    for (int N_ID_1 = 0; N_ID_1 < N_ID_1_NUMBER; N_ID_1++) {

      m0 = 15*(N_ID_1/112) + (5*N_ID_2);
      m1 = N_ID_1%112;

      for (int n = 0; n < LENGTH_SSS_NR; n++) {

        dss_current = (1 - 2*x0[(n + m0)%(LENGTH_SSS_NR)])*(1 - 2*x1[(n + m1)%(LENGTH_SSS_NR)]);

      /* Modulation of SSS is a BPSK TS 36.211 chapter 5.1.2 BPSK */
#if 1
        d_sss[N_ID_2][N_ID_1][n]   = dss_current * amp;
#else
        (void) amp;
        d_sss[N_ID_2][N_ID_1][n]   = (dss_current * SHRT_MAX)>>SCALING_PSS_NR;
#endif
      }
    }
  }

#if 0
  for (int i = 0; i < LENGTH_SSS_NR; i++) {
    printf("sss ref[%i] : %d %d \n", i, d_sss[0][0][i], d_sss[0][0][i]);
  }
#endif
}

/*******************************************************************
*
* NAME :         insert_sss_nr
*
* PARAMETERS :   pointer to input buffer for which sss in inserted
*                amp amplitude applied to input
*                frame parameters for cell identity
*
* RETURN :       none
*
* DESCRIPTION :  Allow to generate a reference sss sequence according to cell identity
*
*********************************************************************/

void insert_sss_nr(int16_t *sss_time,
                   NR_DL_FRAME_PARMS *frame_parms)
{
  unsigned int ofdm_symbol_size = frame_parms->ofdm_symbol_size;
  unsigned int size = ofdm_symbol_size * IQ_SIZE; /* i & q */

  assert(size <= SYNCF_TMP_SIZE);
  assert(size <= SYNC_TMP_SIZE);

  bzero(synchroF_tmp, size);
  bzero(synchro_tmp, size);

  int Nid2 = GET_NID2(frame_parms->Nid_cell);
  int Nid1 = GET_NID1(frame_parms->Nid_cell);

  /* call of IDFT should be done with ordered input as below
    *
    *                n input samples
    *  <------------------------------------------------>
    *  0                                                n
    *  are written into input buffer for IFFT in this order
    *   -------------------------------------------------
    *  |xxxxxxx                       N/2       xxxxxxxx|
    *  --------------------------------------------------
    *  ^      ^                 ^               ^          ^
    *  |      |                 |               |          |
    * n/2    end of            n=0            start of    n/2-1
    *         sss                               sss
    *
    *                   Frequencies
    *      positives                   negatives
    * 0                 (+N/2)(-N/2)
    * |-----------------------><-------------------------|
    *
    * sample 0 is for continuous frequency which is not used here
    */

  unsigned int k = ofdm_symbol_size - ((LENGTH_SSS_NR/2)+1);

  /* SSS is directly mapped to subcarrier */
  for (int i=0; i<LENGTH_SSS_NR; i++) {
    synchroF_tmp[2*k] = d_sss[Nid2][Nid1][i];
    synchroF_tmp[2*k+1] = 0;

    k++;

    if (k >= ofdm_symbol_size) {
      k++;
      k-=ofdm_symbol_size;
    }
  }

  /* get sss in the frequency domain by applying an inverse FFT */
  idft2048(synchroF_tmp,          /* complex input */
           synchro_tmp,           /* complex output */
   	       1);                 /* scaling factor */

  /* then get final sss in time */
  for (unsigned int i=0; i<ofdm_symbol_size; i++) {
    ((int32_t *)sss_time)[i] = ((int32_t *)synchro_tmp)[i];
  }
}

/*******************************************************************
*
* NAME :         pss_ch_est
*
* PARAMETERS :   none
*
* RETURN :       none
*
* DESCRIPTION :  pss channel estimation
*
*********************************************************************/

int pss_ch_est_nr(PHY_VARS_NR_UE *ue,
                  int32_t pss_ext[NB_ANTENNAS_RX][LENGTH_PSS_NR],
                  int32_t sss_ext[NB_ANTENNAS_RX][LENGTH_SSS_NR])
{
  int16_t *pss;
  int16_t *pss_ext2,*sss_ext2,*sss_ext3,tmp_re,tmp_im,tmp_re2,tmp_im2;
  uint8_t aarx,i;
  NR_DL_FRAME_PARMS *frame_parms = &ue->frame_parms;

  pss = primary_synchro_nr[ue->common_vars.eNb_id];

  sss_ext3 = (int16_t*)&sss_ext[0][0];

  for (aarx=0; aarx<frame_parms->nb_antennas_rx; aarx++) {

    sss_ext2 = (int16_t*)&sss_ext[aarx][0];
    pss_ext2 = (int16_t*)&pss_ext[aarx][0];


#if 0

  int16_t *p = pss;

  for (int i = 0; i < LENGTH_PSS_NR; i++) {
    printf(" pss ref [%d]  %d  %d at address %p\n", i, p[2*i], p[2*i+1], &p[2*i]);
    printf(" pss ext [%d]  %d  %d at address %p\n", i, pss_ext2[2*i], pss_ext2[2*i+1], &pss_ext2[2*i]);
  }

#endif

#if 0

  for (int i = 0; i < LENGTH_PSS_NR; i++) {
    printf(" sss ext 2 [%d]  %d  %d at address %p\n", i, sss_ext2[2*i], sss_ext2[2*i+1], &sss_ext2[2*i]);
    printf(" sss ref   [%d]  %d  %d at address %p\n", i, d_sss[0][0][i], d_sss[0][0][i], &d_sss[0][0][i]);
  }

#endif

    for (i = 0; i < LENGTH_PSS_NR; i++) {

      // This is H*(PSS) = R* \cdot PSS
      tmp_re = (int16_t)(((pss_ext2[i*2] * (int32_t)pss[i*2])>>SCALING_CE_PSS_NR)   + ((pss_ext2[i*2+1] * (int32_t)pss[i*2+1])>>SCALING_CE_PSS_NR));
      tmp_im = (int16_t)(((pss_ext2[i*2] * (int32_t)pss[i*2+1])>>SCALING_CE_PSS_NR) - ((pss_ext2[i*2+1] * (int32_t)pss[i*2])>>SCALING_CE_PSS_NR));
      //printf("H*(%d,%d) : (%d,%d)\n",aarx,i,tmp_re,tmp_im);
      //printf("pss(%d,%d) : (%d,%d)\n",aarx,i,pss[2*i],pss[2*i+1]);
      //printf("pss_ext(%d,%d) : (%d,%d)\n",aarx,i,pss_ext2[2*i],pss_ext2[2*i+1]);

      // This is R(SSS) \cdot H*(PSS)
      tmp_re2 = (int16_t)(((tmp_re * (int32_t)sss_ext2[i*2])>>SCALING_CE_PSS_NR)    - ((tmp_im * (int32_t)sss_ext2[i*2+1]>>SCALING_CE_PSS_NR)));
      tmp_im2 = (int16_t)(((tmp_re * (int32_t)sss_ext2[i*2+1])>>SCALING_CE_PSS_NR)  + ((tmp_im * (int32_t)sss_ext2[i*2]>>SCALING_CE_PSS_NR)));

      // printf("SSSi(%d,%d) : (%d,%d)\n",aarx,i,sss_ext2[i<<1],sss_ext2[1+(i<<1)]);
      // printf("SSSo(%d,%d) : (%d,%d)\n",aarx,i,tmp_re2,tmp_im2);
      // MRC on RX antennas
      if (aarx==0) {
        sss_ext3[i<<1]      = tmp_re2;
        sss_ext3[1+(i<<1)]  = tmp_im2;
      } else {
        sss_ext3[i<<1]      += tmp_re2;
        sss_ext3[1+(i<<1)]  += tmp_im2;
      }
    }
  }

#if 0

  for (int i = 0; i < LENGTH_PSS_NR; i++) {
    printf(" sss ext 2 [%d]  %d  %d at address %p\n", i, sss_ext2[2*i], sss_ext2[2*i+1]);
    printf(" sss ref   [%d]  %d  %d at address %p\n", i, d_sss[0][0][i], d_sss[0][0][i]);
    printf(" sss ext 3 [%d]  %d  %d at address %p\n", i, sss_ext3[2*i], sss_ext3[2*i+1]);
  }

#endif

  // sss_ext now contains the compensated SSS
  return(0);
}

/*******************************************************************
*
* NAME :         do_pss_sss_extract
*
* PARAMETERS :   none
*
* RETURN :       none
*
* DESCRIPTION : it allows extracting sss from samples buffer
*
*********************************************************************/

int do_pss_sss_extract_nr(PHY_VARS_NR_UE *ue,
                          int32_t pss_ext[NB_ANTENNAS_RX][LENGTH_PSS_NR],
                          int32_t sss_ext[NB_ANTENNAS_RX][LENGTH_SSS_NR],
                          uint8_t doPss, uint8_t doSss,
					      uint8_t subframe) // add flag to indicate extracting only PSS, only SSS, or both
{
  uint8_t aarx;
  int32_t *pss_rxF,*pss_rxF_ext;
  int32_t *sss_rxF,*sss_rxF_ext;
  uint8_t pss_symbol, sss_symbol;
  int32_t **rxdataF;
  NR_DL_FRAME_PARMS *frame_parms = &ue->frame_parms;

#if 1

  for (aarx=0; aarx<frame_parms->nb_antennas_rx; aarx++) {

    pss_symbol = PSS_SYMBOL_NB;  /* symbol position */
    sss_symbol = SSS_SYMBOL_NB;

    rxdataF  =  ue->common_vars.common_vars_rx_data_per_thread[ue->current_thread_id[subframe]].rxdataF;

    unsigned int ofdm_symbol_size = frame_parms->ofdm_symbol_size;

    pss_rxF  =  &rxdataF[aarx][pss_symbol*ofdm_symbol_size];
    sss_rxF  =  &rxdataF[aarx][sss_symbol*ofdm_symbol_size];

    pss_rxF_ext = &pss_ext[aarx][0];
    sss_rxF_ext = &sss_ext[aarx][0];

    unsigned int k = ofdm_symbol_size - ((LENGTH_PSS_NR/2)+1);

    for (int i=0; i < LENGTH_PSS_NR; i++) {
      if (doPss) {		  
        pss_rxF_ext[i] = pss_rxF[k];
      }

      if (doSss) {
        sss_rxF_ext[i] = sss_rxF[k];
      }

      k++;

      if (k >= ofdm_symbol_size) {
	    k++;
        k-=ofdm_symbol_size;
      }
    }
  }

#if 0

  int16_t *p = (int16_t *)sss_rxF_ext;

  for (int i = 0; i < LENGTH_PSS_NR; i++) {
    printf(" sss ext 2 [%d]  %d  %d at address %p\n", i, p[2*i], p[2*i+1], &p[2*i]);
    printf(" sss ref   [%d]  %d  %d at address %p\n", i, d_sss[0][0][i], d_sss[0][0][i], &d_sss[0][0][i]);
  }

#endif

#if 0

  int16_t *p2 = (int16_t *)pss_rxF_ext;

  for (int i = 0; i < LENGTH_PSS_NR; i++) {
    printf(" pss_rxF_ext [%d]  %d  %d at address %p\n", i, p2[2*i], p2[2*i+1], &p2[2*i]);
  }

#endif

#else

  /* same method as LTE implementation */

  uint16_t rb,nb_rb = N_RB_SS_PBCH_BLOCK;

  int rx_offset = frame_parms->ofdm_symbol_size - (LENGTH_SSS_NR/2);

  for (aarx=0; aarx<frame_parms->nb_antennas_rx; aarx++) {

    pss_symbol = PSS_SYMBOL_NB;
    sss_symbol = SSS_SYMBOL_NB;

    rxdataF  =  ue->common_vars.common_vars_rx_data_per_thread[ue->current_thread_id[subframe]].rxdataF;
    pss_rxF  =  &rxdataF[aarx][(rx_offset + (pss_symbol*(frame_parms->ofdm_symbol_size)))];
    sss_rxF  =  &rxdataF[aarx][(rx_offset + (sss_symbol*(frame_parms->ofdm_symbol_size)))];

    //printf("extract_rbs: symbol_mod=%d, rx_offset=%d, ch_offset=%d\n",symbol_mod,
    //   (rx_offset + (symbol*(frame_parms->ofdm_symbol_size)))*2,
    //   LTE_CE_OFFSET+ch_offset+(symbol_mod*(frame_parms->ofdm_symbol_size)));

    pss_rxF_ext = &pss_ext[aarx][0];
    sss_rxF_ext = &sss_ext[aarx][0];

    for (rb=0; rb<nb_rb; rb++) {

      for (int i=0; i<N_SC_RB; i++) {
        if (doPss) {pss_rxF_ext[i] = pss_rxF[i];}
        if (doSss) {sss_rxF_ext[i] = sss_rxF[i];}
      }

      pss_rxF += N_SC_RB;
      sss_rxF += N_SC_RB;
      pss_rxF_ext += N_SC_RB;
      sss_rxF_ext += N_SC_RB;
    }
  }

#endif

  return(0);
}

/*******************************************************************
*
* NAME :         rx_sss_nr
*
* PARAMETERS :   none
*
* RETURN :       none
*
* DESCRIPTION :
*
*********************************************************************/

int pss_sss_extract_nr(PHY_VARS_NR_UE *phy_vars_ue,
                       int32_t pss_ext[NB_ANTENNAS_RX][LENGTH_PSS_NR],
                       int32_t sss_ext[NB_ANTENNAS_RX][LENGTH_SSS_NR],
					   uint8_t subframe)
{
  return do_pss_sss_extract_nr(phy_vars_ue, pss_ext, sss_ext, 1 /* doPss */, 1 /* doSss */, subframe);
}

/*******************************************************************
*
* NAME :         rx_sss_nr
*
* PARAMETERS :   none
*
* RETURN :       Set Nid_cell in ue context
*
* DESCRIPTION :  Determine element Nid1 of cell identity
*                so Nid_cell in ue context is set according to Nid1 & Nid2
*
*********************************************************************/

int rx_sss_nr(PHY_VARS_NR_UE *ue, int32_t *tot_metric,uint8_t *phase_max)
{
  uint8_t i;
  int32_t pss_ext[NB_ANTENNAS_RX][LENGTH_PSS_NR];
  int32_t sss_ext[NB_ANTENNAS_RX][LENGTH_SSS_NR];
  uint8_t Nid2 = GET_NID2(ue->common_vars.eNb_id);
  uint16_t Nid1;
  uint8_t phase;
  int16_t *sss;
  NR_DL_FRAME_PARMS *frame_parms=&ue->frame_parms;
  int32_t metric, metric_re;
  int16_t *d;

  /* slop_fep function works for lte and takes into account begining of frame with prefix for subframe 0 */
  /* for NR this is not the case but slot_fep is still used for computing FFT of samples */
  /* in order to achieve correct processing for NR prefix samples is forced to 0 and then restored after function call */
  /* symbol number are from beginning of SS/PBCH blocks as below:  */
  /*    Signal            PSS  PBCH  SSS  PBCH                     */
  /*    symbol number      0     1    2    3                       */
  /* time samples in buffer rxdata are used as input of FFT -> FFT results are stored in the frequency buffer rxdataF */
  /* rxdataF stores SS/PBCH from beginning of buffers in the same symbol order as in time domain */

  int nb_prefix_samples0 = frame_parms->nb_prefix_samples0;
  frame_parms->nb_prefix_samples0 = 0;

  // Do FFTs for SSS/PSS
  // SSS
  slot_fep_pbch(ue,
           SSS_SYMBOL_NB,      // symbol number
           0,                  // Ns slot number
           ue->rx_offset,      // sample_offset of int16_t
           0,                  // no_prefix
           1);                 // reset frequency estimation

  // PSS
  slot_fep_pbch(ue,
           PSS_SYMBOL_NB,
           0,
           ue->rx_offset,
           0,
           1);

  frame_parms->nb_prefix_samples0 = nb_prefix_samples0;

  // pss sss extraction
  pss_sss_extract_nr(ue,
                     pss_ext,
                     sss_ext,
                     0);          /* subframe */

#ifdef DEBUG_PLOT_SSS

  write_output("rxsig0.m","rxs0",&ue->common_vars.rxdata[0][0],ue->frame_parms.samples_per_tti,1,1);
  write_output("rxdataF0.m","rxF0",&ue->common_vars.common_vars_rx_data_per_thread[ue->current_thread_id[0]].rxdataF[0],frame_parms->ofdm_symbol_size,2,1);
  write_output("pss_ext.m","pss_ext",pss_ext,LENGTH_PSS_NR,1,1);

#endif

#if 0

  write_output("sss_ext.m","sss_ext",sss_ext,LENGTH_SSS_NR,1,1);
  write_output("sss_ref.m","sss_ref", d_sss,LENGTH_SSS_NR,1,1);

#endif

#if 0
  int16_t *p = (int16_t *)sss_ext[0];
  int16_t *p2 = (int16_t *)pss_ext[0];

  for (int i = 0; i < LENGTH_SSS_NR; i++) {
    printf("sss ref  [%i] : %d %d \n", i, d_sss[0][0][i], d_sss[0][0][i]);
    printf("sss ext  [%i] : %d %d \n", i, p[2*i], p[2*i+1]);

    printf("pss ref [%i] : %d %d \n", i, primary_synchro_nr[0][2*i], primary_synchro_nr[0][2*i+1]);
    printf("pss ext [%i] : %d %d \n", i, p2[2*i], p2[2*i+1]);
  }
#endif

  // get conjugated channel estimate from PSS, H* = R* \cdot PSS
  // and do channel estimation and compensation based on PSS

  pss_ch_est_nr(ue,
                pss_ext,
                sss_ext);

  // now do the SSS detection based on the precomputed sequences in PHY/LTE_TRANSPORT/sss.h
  *tot_metric = INT_MIN;

  sss = (int16_t*)&sss_ext[0][0];

#if 0

  /* simulate of a phase shift on the signal */

  int phase_shift_index = 0;

  phase_shift_samples(sss, LENGTH_SSS_NR, phase_re_nr[phase_shift_index], phase_im_nr[phase_shift_index]);

#endif

#if 0
        int16_t *ps = (int16_t *)pss_ext;

        for (int i = 0; i < LENGTH_SSS_NR; i++) {
          printf("sss ref  [%i] : %d %d \n", i, d_sss[0][0][i], d_sss[0][0][i]);
          printf("sss ext  [%i] : %d %d \n", i, sss[2*i], sss[2*i+1]);

          printf("pss ref [%i] : %d %d \n", i, primary_synchro_nr[0][2*i], primary_synchro_nr[0][2*i+1]);
          printf("pss ext [%i] : %d %d \n", i, ps[2*i], ps[2*i+1]);
        }
#endif

  /* for phase evaluation, one uses an array of possible phase shifts */
  /* then a correlation is done between received signal with a shift pĥase and the reference signal */
  /* Computation of signal with shift phase is based on below formula */
  /* cosinus cos(x + y) = cos(x)cos(y) - sin(x)sin(y) */
  /* sinus   sin(x + y) = sin(x)cos(y) + cos(x)sin(y) */

  for (phase=0; phase < PHASE_HYPOTHESIS_NUMBER; phase++) {  // phase offset between PSS and SSS
    for (Nid1 = 0 ; Nid1 < N_ID_1_NUMBER; Nid1++) {          // all possible Nid1 values

      metric = 0;
      metric_re = 0;

      d = (int16_t *)&d_sss[Nid2][Nid1];

      // This is the inner product using one particular value of each unknown parameter
      for (i=0; i < LENGTH_SSS_NR; i++) {

        metric_re += d[i]*(((phase_re_nr[phase]*sss[2*i])>>SCALING_METRIC_SSS_NR) - ((phase_im_nr[phase]*sss[2*i+1])>>SCALING_METRIC_SSS_NR))
                   + d[i]*(((phase_im_nr[phase]*sss[2*i])>>SCALING_METRIC_SSS_NR) + ((phase_re_nr[phase]*sss[2*i+1])>>SCALING_METRIC_SSS_NR));
      }

      metric = metric_re;

      // if the current metric is better than the last save it
      if (metric > *tot_metric) {
        *tot_metric = metric;
        ue->frame_parms.Nid_cell = Nid2+(3*Nid1);
        *phase_max = phase;

#ifdef DEBUG_SSS_NR

        printf("(phase,Nid1) (%d,%d), metric_phase %d tot_metric %d, phase_max %d \n",phase, Nid1, metric, *tot_metric, *phase_max);

#endif
      }
    }
  }

//#ifdef DEBUG_SSS_NR

#define SSS_METRIC_FLOOR_NR   (30000)
if (*tot_metric > SSS_METRIC_FLOOR_NR) {	
    Nid2 = GET_NID2(frame_parms->Nid_cell);
    Nid1 = GET_NID1(frame_parms->Nid_cell);
    printf("Nid2 %d Nid1 %d tot_metric %d, phase_max %d \n", Nid2, Nid1, *tot_metric, *phase_max);
}
//#endif

  return(0);
}
