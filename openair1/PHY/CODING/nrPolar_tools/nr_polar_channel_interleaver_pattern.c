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

#include "PHY/CODING/nrPolar_tools/nr_polar_defs.h"

void nr_polar_channel_interleaver_pattern(uint16_t *cip, uint8_t I_BIL, uint16_t E){
	
	if (I_BIL == 1) {
		uint16_t T=0, k;
		while( ((T/2)*(T+1)) < E ) T++;
		
		
		int16_t **v = malloc(T * sizeof(*v));
		for (int i = 0; i <= T-1; i++) v[i] = malloc((T-i) * sizeof(*(v[i])));
		
		k=0;
		for (int i = 0; i <= T-1; i++) {
			for (int j = 0; j <= (T-1)-i; j++) {
				if (k<E) {
					v[i][j] = k;
				} else {
					v[i][j] = (-1);
				}
				k++;					
			}
		}
		
		k=0;
		for (int j = 0; j <= T-1; j++) {
					for (int i = 0; i <= (T-1)-j; i++) {
						if ( v[i][j] != (-1) ) {
							cip[k]=v[i][j];
							k++;
						}
					}
				}
		
		for (int i = 0; i <= T-1; i++) free(v[i]);
		free(v);
		
	} else {
		for (int i=0; i<=E-1; i++) cip[i]=i;
	}
	
}
