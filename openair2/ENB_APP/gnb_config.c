#include <string.h>
#include <inttypes.h>

#include "log.h"
#include "log_extern.h"
#include "assertions.h"
#include "gnb_config.h"
#include "UTIL/OTG/otg.h"
#include "UTIL/OTG/otg_externs.h"
#if defined(ENABLE_ITTI)
# include "intertask_interface.h"
# if defined(ENABLE_USE_MME)
#   include "s1ap_eNB.h"
#   include "sctp_eNB_task.h"
# endif
#endif
#include "sctp_default_values.h"
// #include "SystemInformationBlockType2.h"
// #include "LAYER2/MAC/extern.h"
// #include "LAYER2/MAC/proto.h"
#include "PHY/extern.h"
#include "targets/ARCH/ETHERNET/USERSPACE/LIB/ethernet_lib.h"
#include "nfapi_vnf.h"
#include "nfapi_pnf.h"

#include "L1_paramdef.h"
#include "MACRLC_paramdef.h"
#include "common/config/config_userapi.h"
#include "RRC_config_tools.h"
#include "gnb_paramdef.h"


int RCconfig_NRRRC(MessageDef *msg_p, uint32_t i, gNB_RRC_INST *rrc) {

  int                    num_gnbs                                                      = 0;
  int                    num_component_carriers                                        = 0;
  int                    j,k                                                           = 0;
  int32_t                gnb_id                                                        = 0;

  int                    nb_cc                                                         = 0;
  char*                  frame_type                                                    = NULL;
  char*                  DL_prefix_type                                                = NULL;
  char*                  UL_prefix_type                                                = NULL;

  int32_t                eutra_band                                                    = 0;
  long long int          downlink_frequency                                            = 0;
  int32_t                uplink_frequency_offset                                       = 0;
  int32_t                Nid_cell                                                      = 0;
  int32_t                N_RB_DL                                                       = 0;
  int32_t                nb_antenna_ports                                              = 0;

  ///NR
  //MIB
  int32_t                MIB_subCarrierSpacingCommon                                   = 0;
  uint32_t               MIB_ssb_SubcarrierOffset                                      = 0;
  int32_t                MIB_dmrs_TypeA_Position                                       = 0;
  uint32_t               pdcch_ConfigSIB1                                              = 0;

  //SIB1
  int32_t                SIB1_frequencyOffsetSSB                                       = 0; 
  int32_t                SIB1_ssb_PeriodicityServingCell                               = 0;
  int32_t                SIB1_ss_PBCH_BlockPower                                       = 0;
  //NR FrequencyInfoDL
  int32_t                absoluteFrequencySSB                                          = 0;
  uint32_t               ssb_SubcarrierOffset                                          = 0;
  int32_t                DL_FreqBandIndicatorNR                                        = 0;
  int32_t                DL_absoluteFrequencyPointA                                    = 0;

  //NR DL SCS-SpecificCarrier
  uint32_t               DL_offsetToCarrier                                            = 0;
  int32_t                DL_SCS_SubcarrierSpacing                                      = 0;
  int32_t                DL_SCS_SpecificCarrier_k0                                     = 0;
  uint32_t               DL_carrierBandwidth                                           = 0;

  // NR BWP-DownlinkCommon
  uint32_t               DL_locationAndBandwidth                                       = 0;
  long                   DL_BWP_SubcarrierSpacing                                      = 0;
  char*                  DL_BWP_prefix_type                                            = NULL;  

  //NR FrequencyInfoUL
  int32_t                UL_FreqBandIndicatorNR                                        = 0;
  int32_t                UL_absoluteFrequencyPointA                                    = 0;
  uint32_t               UL_additionalSpectrumEmission                                 = 0;
  int32_t                UL_p_Max                                                      = 0;
  int32_t                UL_frequencyShift7p5khz                                       = 0;

  //NR UL SCS-SpecificCarrier
  uint32_t               UL_offsetToCarrier                                            = 0;
  int32_t                UL_SCS_SubcarrierSpacing                                      = 0;
  int32_t                UL_SCS_SpecificCarrier_k0                                     = 0;
  uint32_t               UL_carrierBandwidth                                           = 0;

  // NR BWP-UplinkCommon
  uint32_t               UL_locationAndBandwidth                                       = 0;
  long                   UL_BWP_SubcarrierSpacing                                      = 0;
  char*                  UL_BWP_prefix_type                                            = NULL; 
  
  int32_t                ServingCellConfigCommon_ssb_PositionsInBurst_PR               = 0;
  int32_t                ServingCellConfigCommon_ssb_periodicityServingCell            = 0;
  int32_t                ServingCellConfigCommon_dmrs_TypeA_Position                   = 0;
  int32_t                NIA_SubcarrierSpacing                                         = 0; 
  int32_t                ServingCellConfigCommon_ss_PBCH_BlockPower                    = 0;

  //NR TDD-UL-DL-ConfigCommon
  int32_t                referenceSubcarrierSpacing                                    = 0;
  int32_t                dl_UL_TransmissionPeriodicity                                 = 0;
  int32_t                nrofDownlinkSlots                                             = 0;
  int32_t                nrofDownlinkSymbols                                           = 0;
  int32_t                nrofUplinkSlots                                               = 0;
  int32_t                nrofUplinkSymbols                                             = 0;

  //NR RACH-ConfigCommon
  int32_t                rach_totalNumberOfRA_Preambles                                = 0;
  int32_t                rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_choice         = 0;
  int32_t                rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneEighth      = 0;
  int32_t                rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneFourth      = 0;
  int32_t                rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneHalf        = 0;
  int32_t                rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_one            = 0;
  int32_t                rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_two            = 0;
  uint32_t               rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_four           = 0;
  uint32_t               rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_eight          = 0;
  uint32_t               rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_sixteen        = 0;
  char*                  rach_groupBconfigured                                         = NULL;
  int32_t                rach_ra_Msg3SizeGroupA                                        = 0;
  int32_t                rach_messagePowerOffsetGroupB                                 = 0;
  int32_t                rach_numberOfRA_PreamblesGroupA                               = 0;
  int32_t                rach_ra_ContentionResolutionTimer                             = 0;
  int32_t                rsrp_ThresholdSSB                                             = 0;
  int32_t                rsrp_ThresholdSSB_SUL                                         = 0;
  int32_t                prach_RootSequenceIndex_choice                                = 0;
  uint32_t               prach_RootSequenceIndex_l839                                  = 0;
  uint32_t               prach_RootSequenceIndex_l139                                  = 0;
  int32_t                prach_msg1_SubcarrierSpacing                                  = 0;
  int32_t                restrictedSetConfig                                           = 0;
  int32_t                msg3_transformPrecoding                                       = 0;
  //ssb-perRACH-OccasionAndCB-PreamblesPerSSB not sure

  //NR RACH-ConfigGeneric
  int32_t                prach_ConfigurationIndex                                      = 0;
  int32_t                prach_msg1_FDM                                                = 0;
  int32_t                prach_msg1_FrequencyStart                                     = 0;
  int32_t                zeroCorrelationZoneConfig                                     = 0;
  int32_t                preambleReceivedTargetPower                                   = 0;
  int32_t                preambleTransMax                                              = 0;
  int32_t                powerRampingStep                                              = 0;
  int32_t                ra_ResponseWindow                                             = 0;

  //PUSCH-ConfigCommon
  char*                  groupHoppingEnabledTransformPrecoding                         = NULL;
  int32_t                msg3_DeltaPreamble                                            = 0;
  int32_t                p0_NominalWithGrant                                           = 0;

  ///PUSCH-TimeDomainResourceAllocation
  int32_t                PUSCH_TimeDomainResourceAllocation_k2                         = 0;
  int32_t                PUSCH_TimeDomainResourceAllocation_mappingType                = 0;

  //PUCCH-ConfigCommon
  int32_t                pucch_GroupHopping                                            = 0;
  int32_t                p0_nominal                                                    = 0;

  //PDSCH-ConfigCOmmon
  //PDSCH-TimeDomainResourceAllocation
  int32_t                PDSCH_TimeDomainResourceAllocation_k0                         = 0;
  int32_t                PDSCH_TimeDomainResourceAllocation_mappingType                = 0;

  //RateMatchPattern  is used to configure one rate matching pattern for PDSCH
  int32_t                rateMatchPatternId                                            = 0;
  int32_t                RateMatchPattern_patternType                                  = 0;
  int32_t                symbolsInResourceBlock                                        = 0;
  int32_t                periodicityAndPattern                                         = 0;
  int32_t                RateMatchPattern_controlResourceSet                           = 0;
  int32_t                RateMatchPattern_subcarrierSpacing                            = 0;
  int32_t                RateMatchPattern_mode                                         = 0;

  //PDCCH-ConfigCommon
  int32_t                searchSpaceSIB1                                               = 0;
  int32_t                searchSpaceOtherSystemInformation                             = 0;
  int32_t                pagingSearchSpace                                             = 0;
  int32_t                ra_SearchSpace                                                = 0;
  int32_t                rach_ra_ControlResourceSet                                    = 0;
  //NR PDCCH-ConfigCommon commonControlResourcesSets
  int32_t                PDCCH_common_controlResourceSetId                             = 0;
  int32_t                PDCCH_common_ControlResourceSet_duration                      = 0;
  int32_t                PDCCH_cce_REG_MappingType                                     = 0;
  int32_t                PDCCH_reg_BundleSize                                          = 0;
  int32_t                PDCCH_interleaverSize                                         = 0;
  int32_t                PDCCH_shiftIndex                                              = 0;  
  int32_t                PDCCH_precoderGranularity                                     = 0;
  char*                  tci_PresentInDCI                                              = NULL;

  //NR PDCCH-ConfigCommon commonSearchSpaces
  int32_t                SearchSpaceId                                                 = 0;
  int32_t                commonSearchSpaces_controlResourceSetId                       = 0;
  int32_t                SearchSpace_monitoringSlotPeriodicityAndOffset_choice         = 0;
  int32_t                SearchSpace_monitoringSlotPeriodicityAndOffset_sl1            = 0;
  int32_t                SearchSpace_monitoringSlotPeriodicityAndOffset_sl2            = 0;
  int32_t                SearchSpace_monitoringSlotPeriodicityAndOffset_sl4            = 0;
  int32_t                SearchSpace_monitoringSlotPeriodicityAndOffset_sl5            = 0;
  int32_t                SearchSpace_monitoringSlotPeriodicityAndOffset_sl8            = 0;
  int32_t                SearchSpace_monitoringSlotPeriodicityAndOffset_sl10           = 0;
  int32_t                SearchSpace_monitoringSlotPeriodicityAndOffset_sl16           = 0;
  int32_t                SearchSpace_monitoringSlotPeriodicityAndOffset_sl20           = 0;
  int32_t                SearchSpace_nrofCandidates_aggregationLevel1                  = 0;
  int32_t                SearchSpace_nrofCandidates_aggregationLevel2                  = 0;
  int32_t                SearchSpace_nrofCandidates_aggregationLevel4                  = 0;
  int32_t                SearchSpace_nrofCandidates_aggregationLevel8                  = 0;
  int32_t                SearchSpace_nrofCandidates_aggregationLevel16                 = 0;
  int32_t                SearchSpace_searchSpaceType                                   = 0;
  int32_t                Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel1     = 0;
  int32_t                Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel2     = 0;
  int32_t                Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel4     = 0;
  int32_t                Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel8     = 0;
  int32_t                Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel16    = 0; 
  int32_t                Common_dci_Format2_3_monitoringPeriodicity                    = 0;
  int32_t                Common_dci_Format2_3_nrofPDCCH_Candidates                     = 0;
  int32_t                ue_Specific__dci_Formats                                      = 0;
  //NR  RateMatchPatternLTE-CRS
  uint32_t               RateMatchPatternLTE_CRS_carrierFreqDL                         = 0;
  int32_t                RateMatchPatternLTE_CRS_carrierBandwidthDL                    = 0;
  int32_t                RateMatchPatternLTE_CRS_nrofCRS_Ports                         = 0;
  int32_t                RateMatchPatternLTE_CRS_v_Shift                               = 0;
  int32_t                RateMatchPatternLTE_CRS_radioframeAllocationPeriod            = 0;
  uint32_t               RateMatchPatternLTE_CRS_radioframeAllocationOffset            = 0;
  int32_t                RateMatchPatternLTE_CRS_subframeAllocation_choice             = 0;

  int32_t             my_int;

  paramdef_t GNBSParams[] = GNBSPARAMS_DESC;
  ////////// Identification parameters
  paramdef_t GNBParams[]  = GNBPARAMS_DESC;
  paramlist_def_t GNBParamList = {GNB_CONFIG_STRING_GNB_LIST,NULL,0};
  ////////// Physical parameters
  checkedparam_t config_check_CCparams[] = CCPARAMS_CHECK;
  paramdef_t CCsParams[] = CCPARAMS_DESC;
  paramlist_def_t CCsParamList = {GNB_CONFIG_STRING_COMPONENT_CARRIERS,NULL,0};
  
  paramdef_t SRB1Params[] = SRB1PARAMS_DESC;  

  /* get global parameters, defined outside any section in the config file */
 
  config_get( GNBSParams,sizeof(GNBSParams)/sizeof(paramdef_t),NULL); 
  num_gnbs = GNBSParams[GNB_ACTIVE_GNBS_IDX].numelt;
  AssertFatal (i<num_gnbs,"Failed to parse config file no %ith element in %s \n",i, GNB_CONFIG_STRING_ACTIVE_GNBS);
  
  #if defined(ENABLE_ITTI) && defined(ENABLE_USE_MME)

    if (strcasecmp( *(GNBSParams[GNB_ASN1_VERBOSITY_IDX].strptr), GNB_CONFIG_STRING_ASN1_VERBOSITY_NONE) == 0) {
      asn_debug      = 0;
      asn1_xer_print = 0;
    } else if (strcasecmp( *(GNBSParams[GNB_ASN1_VERBOSITY_IDX].strptr), GNB_CONFIG_STRING_ASN1_VERBOSITY_INFO) == 0) {
      asn_debug      = 1;
      asn1_xer_print = 1;
    } else if (strcasecmp(*(GNBSParams[GNB_ASN1_VERBOSITY_IDX].strptr) , GNB_CONFIG_STRING_ASN1_VERBOSITY_ANNOYING) == 0) {
      asn_debug      = 1;
      asn1_xer_print = 2;
    } else {
      asn_debug      = 0;
      asn1_xer_print = 0;
    }

  #endif

  if (num_gnbs>0) {
    // Output a list of all gNBs. ////////// Identification parameters
    config_getlist( &GNBParamList,GNBParams,sizeof(GNBParams)/sizeof(paramdef_t),NULL); 

    if (GNBParamList.paramarray[i][GNB_GNB_ID_IDX].uptr == NULL) {
    // Calculate a default gNB ID
      # if defined(ENABLE_USE_MME)
        uint32_t hash;
        hash = s1ap_generate_eNB_id ();
        gnb_id = i + (hash & 0xFFFF8);
      # else
        gnb_id = i;
      # endif
    } else {
        gnb_id = *(GNBParamList.paramarray[i][GNB_GNB_ID_IDX].uptr);
    }

    printf("NRRRC %d: Southbound Transport %s\n",i,*(GNBParamList.paramarray[i][GNB_TRANSPORT_S_PREFERENCE_IDX].strptr));

    if (strcmp(*(GNBParamList.paramarray[i][GNB_TRANSPORT_S_PREFERENCE_IDX].strptr), "local_mac") == 0) {
  
    } else if (strcmp(*(GNBParamList.paramarray[i][GNB_TRANSPORT_S_PREFERENCE_IDX].strptr), "cudu") == 0) {
      rrc->eth_params_s.local_if_name            = strdup(*(GNBParamList.paramarray[i][GNB_LOCAL_S_IF_NAME_IDX].strptr));
      rrc->eth_params_s.my_addr                  = strdup(*(GNBParamList.paramarray[i][GNB_LOCAL_S_ADDRESS_IDX].strptr));
      rrc->eth_params_s.remote_addr              = strdup(*(GNBParamList.paramarray[i][GNB_REMOTE_S_ADDRESS_IDX].strptr));
      rrc->eth_params_s.my_portc                 = *(GNBParamList.paramarray[i][GNB_LOCAL_S_PORTC_IDX].uptr);
      rrc->eth_params_s.remote_portc             = *(GNBParamList.paramarray[i][GNB_REMOTE_S_PORTC_IDX].uptr);
      rrc->eth_params_s.my_portd                 = *(GNBParamList.paramarray[i][GNB_LOCAL_S_PORTD_IDX].uptr);
      rrc->eth_params_s.remote_portd             = *(GNBParamList.paramarray[i][GNB_REMOTE_S_PORTD_IDX].uptr);
      rrc->eth_params_s.transp_preference        = ETH_UDP_MODE;
    } else { // other midhaul
    }       

    // search if in active list

    for (k=0; k <num_gnbs ; k++) {
      if (strcmp(GNBSParams[GNB_ACTIVE_GNBS_IDX].strlistptr[k], *(GNBParamList.paramarray[i][GNB_GNB_NAME_IDX].strptr) )== 0) {
        
        char gnbpath[MAX_OPTNAME_SIZE + 8];

        NRRRC_CONFIGURATION_REQ (msg_p).cell_identity     = gnb_id;
        NRRRC_CONFIGURATION_REQ (msg_p).tac               = (uint16_t)atoi( *(GNBParamList.paramarray[i][GNB_TRACKING_AREA_CODE_IDX].strptr) );
        NRRRC_CONFIGURATION_REQ (msg_p).mcc               = (uint16_t)atoi( *(GNBParamList.paramarray[i][GNB_MOBILE_COUNTRY_CODE_IDX].strptr) );
        NRRRC_CONFIGURATION_REQ (msg_p).mnc               = (uint16_t)atoi( *(GNBParamList.paramarray[i][GNB_MOBILE_NETWORK_CODE_IDX].strptr) );
        NRRRC_CONFIGURATION_REQ (msg_p).mnc_digit_length  = strlen(*(GNBParamList.paramarray[i][GNB_MOBILE_NETWORK_CODE_IDX].strptr));
        AssertFatal((NRRRC_CONFIGURATION_REQ (msg_p).mnc_digit_length == 2) ||
                    (NRRRC_CONFIGURATION_REQ (msg_p).mnc_digit_length == 3),"BAD MNC DIGIT LENGTH %d",
                     NRRRC_CONFIGURATION_REQ (msg_p).mnc_digit_length);

        // Parse optional physical parameters
        sprintf(gnbpath,"%s.[%i]",GNB_CONFIG_STRING_GNB_LIST,k),
        config_getlist( &CCsParamList,NULL,0,gnbpath); 
    
        LOG_I(NRRRC,"num component carriers %d \n", num_component_carriers); 

        if ( CCsParamList.numelt> 0) {
          
          char ccspath[MAX_OPTNAME_SIZE*2 + 16];

          for (j = 0; j < CCsParamList.numelt ;j++) {
            
            sprintf(ccspath,"%s.%s.[%i]",gnbpath,GNB_CONFIG_STRING_COMPONENT_CARRIERS,j);
            config_get( CCsParams,sizeof(CCsParams)/sizeof(paramdef_t),ccspath);        
 
            nb_cc++;

            // NRRRC_CONFIGURATION_REQ (msg_p).tdd_config[j] = tdd_config;
            // AssertFatal (tdd_config <= TDD_Config__subframeAssignment_sa6,
            //             "Failed to parse gNB configuration file %s, gnb %d illegal tdd_config %d (should be 0-%d)!",
            //             RC.config_file_name, i, tdd_config, TDD_Config__subframeAssignment_sa6);
        
            // NRRRC_CONFIGURATION_REQ (msg_p).tdd_config_s[j] = tdd_config_s;
            // AssertFatal (tdd_config_s <= TDD_Config__specialSubframePatterns_ssp8,
            //             "Failed to parse gNB configuration file %s, gnb %d illegal tdd_config_s %d (should be 0-%d)!",
            //             RC.config_file_name, i, tdd_config_s, TDD_Config__specialSubframePatterns_ssp8);

            if (!DL_prefix_type){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d define %s: NORMAL,EXTENDED!\n",
                           RC.config_file_name, i, GNB_CONFIG_STRING_DL_PREFIX_TYPE);
            }else if (strcmp(DL_prefix_type, "NORMAL") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).DL_prefix_type[j] = NORMAL;
            }else if (strcmp(DL_prefix_type, "EXTENDED") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).DL_prefix_type[j] = EXTENDED;
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%s\" for DL_prefix_type choice: NORMAL or EXTENDED !\n",
                           RC.config_file_name, i, DL_prefix_type);
            }

            if (!UL_prefix_type){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d define %s: NORMAL,EXTENDED!\n",
                           RC.config_file_name, i, GNB_CONFIG_STRING_UL_PREFIX_TYPE);
            }else if (strcmp(UL_prefix_type, "NORMAL") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).UL_prefix_type[j] = NORMAL;
            }else if (strcmp(UL_prefix_type, "EXTENDED") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).UL_prefix_type[j] = EXTENDED;
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%s\" for UL_prefix_type choice: NORMAL or EXTENDED !\n",
                           RC.config_file_name, i, UL_prefix_type);
            }            

            NRRRC_CONFIGURATION_REQ (msg_p).eutra_band[j] = eutra_band;
            NRRRC_CONFIGURATION_REQ (msg_p).downlink_frequency[j] = (uint32_t) downlink_frequency;
            NRRRC_CONFIGURATION_REQ (msg_p).uplink_frequency_offset[j] = (unsigned int) uplink_frequency_offset;
            NRRRC_CONFIGURATION_REQ (msg_p).Nid_cell[j]= Nid_cell;

            if (Nid_cell>503) {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for Nid_cell choice: 0...503 !\n",
                           RC.config_file_name, i, Nid_cell);
            }
        
            NRRRC_CONFIGURATION_REQ (msg_p).N_RB_DL[j]= N_RB_DL;
        
            if ((N_RB_DL!=6) && (N_RB_DL!=15) && (N_RB_DL!=25) && (N_RB_DL!=50) && (N_RB_DL!=75) && (N_RB_DL!=100)) {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for N_RB_DL choice: 6,15,25,50,75,100 !\n",
                           RC.config_file_name, i, N_RB_DL);
            }
        
            if (strcmp(frame_type, "FDD") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).frame_type[j] = FDD;
            }else  if (strcmp(frame_type, "TDD") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).frame_type[j] = TDD;
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%s\" for frame_type choice: FDD or TDD !\n",
                           RC.config_file_name, i, frame_type);
            }

            if (!prefix_type){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d define %s: NORMAL,EXTENDED!\n",
                           RC.config_file_name, i, ENB_CONFIG_STRING_PREFIX_TYPE);
            }else if (strcmp(prefix_type, "NORMAL") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).prefix_type[j] = NORMAL;
            }else  if (strcmp(prefix_type, "EXTENDED") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).prefix_type[j] = EXTENDED;
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%s\" for prefix_type choice: NORMAL or EXTENDED !\n",
                           RC.config_file_name, i, prefix_type);
            }

            NRRRC_CONFIGURATION_REQ (msg_p).eutra_band[j] = eutra_band;
            NRRRC_CONFIGURATION_REQ (msg_p).downlink_frequency[j] = (uint32_t) downlink_frequency;
            NRRRC_CONFIGURATION_REQ (msg_p).uplink_frequency_offset[j] = (unsigned int) uplink_frequency_offset;
        
            if (config_check_band_frequencies(j,
              NRRRC_CONFIGURATION_REQ (msg_p).eutra_band[j],
              NRRRC_CONFIGURATION_REQ (msg_p).downlink_frequency[j],
              NRRRC_CONFIGURATION_REQ (msg_p).uplink_frequency_offset[j],
              NRRRC_CONFIGURATION_REQ (msg_p).frame_type[j])) {
               AssertFatal(0, "error calling enb_check_band_frequencies\n");
            }

            if ((nb_antenna_ports <1) || (nb_antenna_ports > 2)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, enb %d unknown value \"%d\" for nb_antenna_ports choice: 1..2 !\n",
                           RC.config_file_name, i, nb_antenna_ports);
            }
            NRRRC_CONFIGURATION_REQ (msg_p).nb_antenna_ports[j] = nb_antenna_ports;


            ////////////////////////////////////////////////////////////////////////////////
            //---------------------------NR--------Configuration--------------------------//
            ////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////MIB///////////////////////////////
            NRRRC_CONFIGURATION_REQ (msg_p).MIB_subCarrierSpacingCommon[j] = MIB_subCarrierSpacingCommon;
            if ((MIB_subCarrierSpacingCommon !=15) && (MIB_subCarrierSpacingCommon !=30) && (MIB_subCarrierSpacingCommon !=60) && (MIB_subCarrierSpacingCommon !=120)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for MIB_subCarrierSpacingCommon choice: 15,30,60,120 !\n",
                           RC.config_file_name, i, MIB_subCarrierSpacingCommon);
            }

            NRRRC_CONFIGURATION_REQ (msg_p).MIB_ssb_SubcarrierOffset[j] = MIB_ssb_SubcarrierOffset;
            if ((MIB_ssb_SubcarrierOffset <0) || (MIB_ssb_SubcarrierOffset > 15)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for MIB_ssb_SubcarrierOffset choice: 1..23 !\n",
                           RC.config_file_name, i, MIB_ssb_SubcarrierOffset);
            }

            NRRRC_CONFIGURATION_REQ (msg_p).MIB_dmrs_TypeA_Position[j] = MIB_dmrs_TypeA_Position;
            if ((MIB_dmrs_TypeA_Position !=2) && (MIB_dmrs_TypeA_Position !=3)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for MIB_dmrs_TypeA_Position choice:2,3 !\n",
                           RC.config_file_name, i, MIB_dmrs_TypeA_Position);
            }          

            NRRRC_CONFIGURATION_REQ (msg_p).pdcch_ConfigSIB1[j] = pdcch_ConfigSIB1;
            if ((pdcch_ConfigSIB1 <0) || (pdcch_ConfigSIB1 > 255)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for pdcch_ConfigSIB1 choice: 0..3279165 !\n",
                           RC.config_file_name, i, pdcch_ConfigSIB1);
            }
            
            ////////////////////////////////SIB1//////////////////////////////

            if (strcmp(SIB1_frequencyOffsetSSB , "khz-5") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).SIB1_frequencyOffsetSSB[j] = -5;                    
            }else if (strcmp(SIB1_frequencyOffsetSSB , "khz5") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).SIB1_frequencyOffsetSSB[j] = 5;
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for SIB1_frequencyOffsetSSB !\n",
                           RC.config_file_name, i, SIB1_frequencyOffsetSSB);
            }

            NRRRC_CONFIGURATION_REQ (msg_p).SIB1_ssb_PeriodicityServingCell[j] = SIB1_ssb_PeriodicityServingCell;
            if ((SIB1_ssb_PeriodicityServingCell !=5)  && 
                (SIB1_ssb_PeriodicityServingCell !=10) && 
                (SIB1_ssb_PeriodicityServingCell !=20) && 
                (SIB1_ssb_PeriodicityServingCell !=40) &&
                (SIB1_ssb_PeriodicityServingCell !=80) &&
                (SIB1_ssb_PeriodicityServingCell !=160)&&){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for SIB1_ssb_PeriodicityServingCell choice: 5,10,20,40,80,160 !\n",
                           RC.config_file_name, i, SIB1_ssb_PeriodicityServingCell);
            }            

            NRRRC_CONFIGURATION_REQ (msg_p).SIB1_ss_PBCH_BlockPower[j] = SIB1_ss_PBCH_BlockPower;
            if ((SIB1_ss_PBCH_BlockPower < -60) || (SIB1_ss_PBCH_BlockPower > 50)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for SIB1_ss_PBCH_BlockPower choice: -60..50 !\n",
              RC.config_file_name, i, SIB1_ss_PBCH_BlockPower);
            }

            ////////////////////////////////NR FrequencyInfoDL//////////////////////////////
            NRRRC_CONFIGURATION_REQ (msg_p).absoluteFrequencySSB[j] = absoluteFrequencySSB;
            if ((absoluteFrequencySSB <0) || (absoluteFrequencySSB > 3279165)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for absoluteFrequencySSB choice: 0..3279165 !\n",
                           RC.config_file_name, i, absoluteFrequencySSB);
            }

            NRRRC_CONFIGURATION_REQ (msg_p).ssb_SubcarrierOffset[j] = ssb_SubcarrierOffset;
            if ((ssb_SubcarrierOffset <1) || (ssb_SubcarrierOffset > 23)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for ssb_SubcarrierOffset choice: 1..23 !\n",
                           RC.config_file_name, i, ssb_SubcarrierOffset);
            }

            NRRRC_CONFIGURATION_REQ (msg_p).DL_FreqBandIndicatorNR[j] = DL_FreqBandIndicatorNR;
            if ((DL_FreqBandIndicatorNR <1) || (DL_FreqBandIndicatorNR > 1024)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for DL_FreqBandIndicatorNR choice: 1..1024 !\n",
                           RC.config_file_name, i, DL_FreqBandIndicatorNR);
            }

            NRRRC_CONFIGURATION_REQ (msg_p).DL_absoluteFrequencyPointA[j] = DL_absoluteFrequencyPointA;
            if ((DL_absoluteFrequencyPointA <0) || (DL_absoluteFrequencyPointA > 3279165)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for DL_absoluteFrequencyPointA choice: 0..3279165 !\n",
                           RC.config_file_name, i, DL_absoluteFrequencyPointA);
            }            
            

            /////////////////////////////////NR DL SCS-SpecificCarrier///////////////////////////
            NRRRC_CONFIGURATION_REQ (msg_p).DL_offsetToCarrier[j] = DL_offsetToCarrier;
            if ((DL_offsetToCarrier <0) || (DL_offsetToCarrier > 2199)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for DL_offsetToCarrier choice: 0..11 !\n",
                           RC.config_file_name, i, DL_offsetToCarrier);
            }

            if (strcmp(DL_SCS_SubcarrierSpacing,"kHz15")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).DL_SCS_SubcarrierSpacing[j] = SubcarrierSpacing_kHz15;
            }else if (strcmp(DL_SCS_SubcarrierSpacing,"kHz30")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).DL_SCS_SubcarrierSpacing[j] = SubcarrierSpacing_kHz30;
            }else if (strcmp(DL_SCS_SubcarrierSpacing,"kHz60")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).DL_SCS_SubcarrierSpacing[j] = SubcarrierSpacing_kHz60;
            }else if (strcmp(DL_SCS_SubcarrierSpacing,"kHz120")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).DL_SCS_SubcarrierSpacing[j] = SubcarrierSpacing_kHz120;
            }else if (strcmp(DL_SCS_SubcarrierSpacing,"kHz240")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).DL_SCS_SubcarrierSpacing[j] = SubcarrierSpacing_kHz240;
            }else { 
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%s\" for DL_SCS_SubcarrierSpacing choice: minusinfinity,kHz15,kHz30,kHz60,kHz120,kHz240!\n",
                           RC.config_file_name, i, DL_SCS_SubcarrierSpacing);
            }

            switch (DL_SCS_SpecificCarrier_k0) {
              case -6:
                NRRRC_CONFIGURATION_REQ (msg_p).DL_SCS_SpecificCarrier_k0[j] =  SCS_SpecificCarrier__k0_n_6;
                break;

              case 0:
                NRRRC_CONFIGURATION_REQ (msg_p).DL_SCS_SpecificCarrier_k0[j] =  SCS_SpecificCarrier__k0_n0;
                break;

              case 6:
                NRRRC_CONFIGURATION_REQ (msg_p).DL_SCS_SpecificCarrier_k0[j] =  SCS_SpecificCarrier__k0_n6;
                break;

               default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for DL_SCS_SpecificCarrier_k0 choice: -6,0,6 !\n",
                             RC.config_file_name, i, DL_SCS_SpecificCarrier_k0);
                break;
            }

            NRRRC_CONFIGURATION_REQ (msg_p).DL_carrierBandwidth[j] = DL_carrierBandwidth;
            if ((DL_carrierBandwidth <1) || (DL_carrierBandwidth > 275)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for DL_carrierBandwidth choice: 1..275 !\n",
              RC.config_file_name, i, DL_carrierBandwidth);
            }

            /////////////////////////////////NR BWP-DownlinkCommon///////////////////////////
            NRRRC_CONFIGURATION_REQ (msg_p).DL_locationAndBandwidth[j] = DL_locationAndBandwidth;
            if ((DL_locationAndBandwidth <0) || (DL_locationAndBandwidth > 37949)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for DL_locationAndBandwidth choice: 0..11 !\n",
              RC.config_file_name, i, DL_locationAndBandwidth);
            }

            if (strcmp(DL_BWP_SubcarrierSpacing,"kHz15")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).DL_BWP_SubcarrierSpacing[j] = SubcarrierSpacing_kHz15;
            }else if (strcmp(DL_BWP_SubcarrierSpacing,"kHz30")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).DL_BWP_SubcarrierSpacing[j] = SubcarrierSpacing_kHz30;
            }else if (strcmp(DL_BWP_SubcarrierSpacing,"kHz60")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).DL_BWP_SubcarrierSpacing[j] = SubcarrierSpacing_kHz60;
            }else if (strcmp(DL_BWP_SubcarrierSpacing,"kHz120")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).DL_BWP_SubcarrierSpacing[j] = SubcarrierSpacing_kHz120;
            }else if (strcmp(DL_BWP_SubcarrierSpacing,"kHz240")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).DL_BWP_SubcarrierSpacing[j] = SubcarrierSpacing_kHz240;
            }else { 
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%s\" for DL_BWP_SubcarrierSpacing choice: minusinfinity,kHz15,kHz30,kHz60,kHz120,kHz240!\n",
                           RC.config_file_name, i, DL_BWP_SubcarrierSpacing);
            }

            if (!DL_BWP_prefix_type){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d define %s: NORMAL,EXTENDED!\n",
                           RC.config_file_name, i, GNB_CONFIG_STRING_DL_PREFIX_TYPE);
            }else if (strcmp(DL_BWP_prefix_type, "NORMAL") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).DL_BWP_prefix_type[j] = NORMAL;
            }else if (strcmp(DL_BWP_prefix_type, "EXTENDED") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).DL_BWP_prefix_type[j] = EXTENDED;
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%s\" for DL_BWP_prefix_type choice: NORMAL or EXTENDED !\n",
                           RC.config_file_name, i, DL_BWP_prefix_type);
            }                        

            /////////////////////////////////NR FrequencyInfoUL//////////////////////////////
            NRRRC_CONFIGURATION_REQ (msg_p).UL_FreqBandIndicatorNR[j] = UL_FreqBandIndicatorNR;
            if ((UL_FreqBandIndicatorNR <1) || (UL_FreqBandIndicatorNR > 1024)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for UL_FreqBandIndicatorNR choice: 1..1024 !\n",
                           RC.config_file_name, i, UL_FreqBandIndicatorNR);
            }

            NRRRC_CONFIGURATION_REQ (msg_p).UL_absoluteFrequencyPointA[j] = UL_absoluteFrequencyPointA;
            if ((UL_absoluteFrequencyPointA <0) || (UL_absoluteFrequencyPointA > 3279165)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for UL_absoluteFrequencyPointA choice: 0..3279165 !\n",
                           RC.config_file_name, i, UL_absoluteFrequencyPointA);
            }       

            NRRRC_CONFIGURATION_REQ (msg_p).UL_additionalSpectrumEmission[j] = UL_additionalSpectrumEmission;
            if ((UL_additionalSpectrumEmission <0) || (UL_additionalSpectrumEmission > 7)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for UL_additionalSpectrumEmission choice: 0..7 !\n",
                           RC.config_file_name, i, UL_additionalSpectrumEmission);
            }            

            NRRRC_CONFIGURATION_REQ (msg_p).UL_p_Max[j] = UL_p_Max;
            if ((UL_p_Max <-30) || (UL_p_Max > 33)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for UL_p_Max choice: -30..33 !\n",
              RC.config_file_name, i, UL_p_Max);
            }

            if (strcmp(UL_frequencyShift7p5khz, "TRUE") == 0) {
              RRC_CONFIGURATION_REQ (msg_p).UL_frequencyShift7p5khz[j] = FrequencyInfoUL__frequencyShift7p5khz_true; //enum true = 0
            }else if{
              RRC_CONFIGURATION_REQ (msg_p).UL_frequencyShift7p5khz[j] = 1;//false               
            } 

            /////////////////////////////////NR UL SCS-SpecificCarrier///////////////////////////
            NRRRC_CONFIGURATION_REQ (msg_p).UL_offsetToCarrier[j] = UL_offsetToCarrier;
            if ((UL_offsetToCarrier <0) || (UL_offsetToCarrier > 2199)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for UL_offsetToCarrier choice: 0..11 !\n",
              RC.config_file_name, i, UL_offsetToCarrier);
            }

            if (strcmp(UL_SCS_SubcarrierSpacing,"kHz15")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).UL_SCS_SubcarrierSpacing[j] = SubcarrierSpacing_kHz15;
            }else if (strcmp(UL_SCS_SubcarrierSpacing,"kHz30")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).UL_SCS_SubcarrierSpacing[j] = SubcarrierSpacing_kHz30;
            }else if (strcmp(UL_SCS_SubcarrierSpacing,"kHz60")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).UL_SCS_SubcarrierSpacing[j] = SubcarrierSpacing_kHz60;
            }else if (strcmp(UL_SCS_SubcarrierSpacing,"kHz120")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).UL_SCS_SubcarrierSpacing[j] = SubcarrierSpacing_kHz120;
            }else if (strcmp(UL_SCS_SubcarrierSpacing,"kHz240")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).UL_SCS_SubcarrierSpacing[j] = SubcarrierSpacing_kHz240;
            }else { AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%s\" for UL_SCS_SubcarrierSpacing choice: minusinfinity,kHz15,kHz30,kHz60,kHz120,kHz240!\n",
              RC.config_file_name, i, UL_SCS_SubcarrierSpacing);
            }

            switch (UL_SCS_SpecificCarrier_k0) {
              case -6:
                NRRRC_CONFIGURATION_REQ (msg_p).UL_SCS_SpecificCarrier_k0[j] =  SCS_SpecificCarrier__k0_n_6;
                break;

              case 0:
                NRRRC_CONFIGURATION_REQ (msg_p).UL_SCS_SpecificCarrier_k0[j] =  SCS_SpecificCarrier__k0_n0;
                break;

              case 6:
                NRRRC_CONFIGURATION_REQ (msg_p).UL_SCS_SpecificCarrier_k0[j] =  SCS_SpecificCarrier__k0_n6;
                break;

               default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for UL_SCS_SpecificCarrier_k0 choice: -6,0,6 !\n",
                             RC.config_file_name, i, UL_SCS_SpecificCarrier_k0);
                break;
            }

            NRRRC_CONFIGURATION_REQ (msg_p).UL_carrierBandwidth[j] = UL_carrierBandwidth;
            if ((UL_carrierBandwidth <1) || (UL_carrierBandwidth > 275)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for UL_carrierBandwidth choice: 1..275 !\n",
              RC.config_file_name, i, UL_carrierBandwidth);
            }



            //Not Sure the value selection
            NRRRC_CONFIGURATION_REQ (msg_p).absoluteFrequencyPointA[j] = absoluteFrequencyPointA;
                        
            NRRRC_CONFIGURATION_REQ (msg_p).ControlResourceSetId[j] = ControlResourceSetId;
            if ((ControlResourceSetId <0) || (ControlResourceSetId > 11)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for ControlResourceSetId choice: 0..11 !\n",
              RC.config_file_name, i, ControlResourceSetId);
            }

            /////////////////////////////////NR BWP-UplinkCommon///////////////////////////
            NRRRC_CONFIGURATION_REQ (msg_p).UL_locationAndBandwidth[j] = UL_locationAndBandwidth;
            if ((UL_locationAndBandwidth <0) || (UL_locationAndBandwidth > 37949)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for UL_locationAndBandwidth choice: 0..11 !\n",
              RC.config_file_name, i, UL_locationAndBandwidth);
            }

            if (strcmp(UL_BWP_SubcarrierSpacing,"kHz15")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).UL_BWP_SubcarrierSpacing[j] = SubcarrierSpacing_kHz15;
            }else if (strcmp(UL_BWP_SubcarrierSpacing,"kHz30")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).UL_BWP_SubcarrierSpacing[j] = SubcarrierSpacing_kHz30;
            }else if (strcmp(UL_BWP_SubcarrierSpacing,"kHz60")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).UL_BWP_SubcarrierSpacing[j] = SubcarrierSpacing_kHz60;
            }else if (strcmp(UL_BWP_SubcarrierSpacing,"kHz120")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).UL_BWP_SubcarrierSpacing[j] = SubcarrierSpacing_kHz120;
            }else if (strcmp(UL_BWP_SubcarrierSpacing,"kHz240")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).UL_BWP_SubcarrierSpacing[j] = SubcarrierSpacing_kHz240;
            }else { 
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%s\" for UL_BWP_SubcarrierSpacing choice: minusinfinity,kHz15,kHz30,kHz60,kHz120,kHz240!\n",
                           RC.config_file_name, i, UL_BWP_SubcarrierSpacing);
            }

            if (!UL_BWP_prefix_type){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d define %s: NORMAL,EXTENDED!\n",
                           RC.config_file_name, i, GNB_CONFIG_STRING_DL_PREFIX_TYPE);
            }else if (strcmp(UL_BWP_prefix_type, "NORMAL") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).UL_BWP_prefix_type[j] = NORMAL;
            }else if (strcmp(UL_BWP_prefix_type, "EXTENDED") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).UL_BWP_prefix_type[j] = EXTENDED;
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%s\" for UL_BWP_prefix_type choice: NORMAL or EXTENDED !\n",
                           RC.config_file_name, i, UL_BWP_prefix_type);
            }  

            if (strcmp(ServingCellConfigCommon_ssb_PositionsInBurst_PR,"shortBitmap")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).ServingCellConfigCommon_ssb_PositionsInBurst_PR[j] = ServingCellConfigCommon__ssb_PositionsInBurst_PR_shortBitmap;
            }else if (strcmp(ServingCellConfigCommon_ssb_PositionsInBurst_PR,"mediumBitmap")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).ServingCellConfigCommon_ssb_PositionsInBurst_PR[j] = ServingCellConfigCommon__ssb_PositionsInBurst_PR_mediumBitmap;
            }else if (strcmp(ServingCellConfigCommon_ssb_PositionsInBurst_PR,"longBitmap")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).ServingCellConfigCommon_ssb_PositionsInBurst_PR[j] = ServingCellConfigCommon__ssb_PositionsInBurst_PR_longBitmap;
            }else if (strcmp(ServingCellConfigCommon_ssb_PositionsInBurst_PR,"NOTHING")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).ServingCellConfigCommon_ssb_PositionsInBurst_PR[j] = ServingCellConfigCommon__ssb_PositionsInBurst_PR_NOTHING;
            }else { 
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%s\" for ServingCellConfigCommon_ssb_PositionsInBurst_PR choice !\n",
                           RC.config_file_name, i, ServingCellConfigCommon_ssb_PositionsInBurst_PR);
            }            


            switch (ServingCellConfigCommon_ssb_periodicityServingCell) {
              case 5:
                NRRRC_CONFIGURATION_REQ (msg_p).ServingCellConfigCommon_ssb_periodicityServingCell[j] =  ServingCellConfigCommon__ssb_periodicityServingCell_ms5;
                break;

              case 10:
                NRRRC_CONFIGURATION_REQ (msg_p).ServingCellConfigCommon_ssb_periodicityServingCell[j] =  ServingCellConfigCommon__ssb_periodicityServingCell_ms10;
                break;

              case 20:
                NRRRC_CONFIGURATION_REQ (msg_p).ServingCellConfigCommon_ssb_periodicityServingCell[j] =  ServingCellConfigCommon__ssb_periodicityServingCell_ms20;
                break;
              
              case 40:
                NRRRC_CONFIGURATION_REQ (msg_p).ServingCellConfigCommon_ssb_periodicityServingCell[j] =  ServingCellConfigCommon__ssb_periodicityServingCell_ms40;
                break;
                          
              case 80:
                NRRRC_CONFIGURATION_REQ (msg_p).ServingCellConfigCommon_ssb_periodicityServingCell[j] =  ServingCellConfigCommon__ssb_periodicityServingCell_ms80;
                break;

              case 160:
                NRRRC_CONFIGURATION_REQ (msg_p).ServingCellConfigCommon_ssb_periodicityServingCell[j] =  ServingCellConfigCommon__ssb_periodicityServingCell_ms160;
                break;

               default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for ServingCellConfigCommon_ssb_periodicityServingCell choice: -6,0,6 !\n",
                             RC.config_file_name, i, ServingCellConfigCommon_ssb_periodicityServingCell);
                break;
            }

            switch (ServingCellConfigCommon_dmrs_TypeA_Position) {
              case 2:
                NRRRC_CONFIGURATION_REQ (msg_p).ServingCellConfigCommon_dmrs_TypeA_Position[j] =  ServingCellConfigCommon__dmrs_TypeA_Position_pos2;
                break;

              case 3:
                NRRRC_CONFIGURATION_REQ (msg_p).ServingCellConfigCommon_dmrs_TypeA_Position[j] =  ServingCellConfigCommon__dmrs_TypeA_Position_pos3;
                break;

               default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for ServingCellConfigCommon_dmrs_TypeA_Position choice: -6,0,6 !\n",
                             RC.config_file_name, i, ServingCellConfigCommon_dmrs_TypeA_Position);
                break;
            }

            if (strcmp(NIA_SubcarrierSpacing,"kHz15")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).NIA_SubcarrierSpacing[j] = SubcarrierSpacing_kHz15;
            }else if (strcmp(NIA_SubcarrierSpacing,"kHz30")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).NIA_SubcarrierSpacing[j] = SubcarrierSpacing_kHz30;
            }else if (strcmp(NIA_SubcarrierSpacing,"kHz60")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).NIA_SubcarrierSpacing[j] = SubcarrierSpacing_kHz60;
            }else if (strcmp(NIA_SubcarrierSpacing,"kHz120")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).NIA_SubcarrierSpacing[j] = SubcarrierSpacing_kHz120;
            }else if (strcmp(NIA_SubcarrierSpacing,"kHz240")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).NIA_SubcarrierSpacing[j] = SubcarrierSpacing_kHz240;
            }else { AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%s\" for NIA_SubcarrierSpacing choice: minusinfinity,kHz15,kHz30,kHz60,kHz120,kHz240!\n",
              RC.config_file_name, i, NIA_SubcarrierSpacing);
            }

            NRRRC_CONFIGURATION_REQ (msg_p).ServingCellConfigCommon_ss_PBCH_BlockPower[j] = ServingCellConfigCommon_ss_PBCH_BlockPower;
            if ((ServingCellConfigCommon_ss_PBCH_BlockPower < -60) || (ServingCellConfigCommon_ss_PBCH_BlockPower > 50)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for ServingCellConfigCommon_ss_PBCH_BlockPower choice: 0..11 !\n",
              RC.config_file_name, i, ServingCellConfigCommon_ss_PBCH_BlockPower);
            }

            /////////////////////////////////NR TDD-UL-DL-ConfigCommon///////////////////////////
            if (strcmp(referenceSubcarrierSpacing,"kHz15")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).referenceSubcarrierSpacing[j] = SubcarrierSpacing_kHz15;
            }else if (strcmp(referenceSubcarrierSpacing,"kHz30")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).referenceSubcarrierSpacing[j] = SubcarrierSpacing_kHz30;
            }else if (strcmp(referenceSubcarrierSpacing,"kHz60")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).referenceSubcarrierSpacing[j] = SubcarrierSpacing_kHz60;
            }else if (strcmp(referenceSubcarrierSpacing,"kHz120")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).referenceSubcarrierSpacing[j] = SubcarrierSpacing_kHz120;
            }else if (strcmp(referenceSubcarrierSpacing,"kHz240")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).referenceSubcarrierSpacing[j] = SubcarrierSpacing_kHz240;
            }else { 
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%s\" for referenceSubcarrierSpacing choice: minusinfinity,kHz15,kHz30,kHz60,kHz120,kHz240!\n",
                  RC.config_file_name, i, referenceSubcarrierSpacing);
            }

            if (strcmp(dl_UL_TransmissionPeriodicity,"ms0p5")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).dl_UL_TransmissionPeriodicity[j] = TDD_UL_DL_ConfigCommon__dl_UL_TransmissionPeriodicity_ms0p5;
            }else if (strcmp(dl_UL_TransmissionPeriodicity,"ms0p625")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).dl_UL_TransmissionPeriodicity[j] = TDD_UL_DL_ConfigCommon__dl_UL_TransmissionPeriodicity_ms0p625;
            }else if (strcmp(dl_UL_TransmissionPeriodicity,"ms1")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).dl_UL_TransmissionPeriodicity[j] = TDD_UL_DL_ConfigCommon__dl_UL_TransmissionPeriodicity_ms1;
            }else if (strcmp(dl_UL_TransmissionPeriodicity,"ms1p25")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).dl_UL_TransmissionPeriodicity[j] = TDD_UL_DL_ConfigCommon__dl_UL_TransmissionPeriodicity_ms1p25;
            }else if (strcmp(dl_UL_TransmissionPeriodicity,"ms2")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).dl_UL_TransmissionPeriodicity[j] = TDD_UL_DL_ConfigCommon__dl_UL_TransmissionPeriodicity_ms2;
            }else if (strcmp(dl_UL_TransmissionPeriodicity,"ms2p5")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).dl_UL_TransmissionPeriodicity[j] = TDD_UL_DL_ConfigCommon__dl_UL_TransmissionPeriodicity_ms2p5;
            }else if (strcmp(dl_UL_TransmissionPeriodicity,"ms5")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).dl_UL_TransmissionPeriodicity[j] = TDD_UL_DL_ConfigCommon__dl_UL_TransmissionPeriodicity_ms5;
            }else if (strcmp(dl_UL_TransmissionPeriodicity,"ms10")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).dl_UL_TransmissionPeriodicity[j] = TDD_UL_DL_ConfigCommon__dl_UL_TransmissionPeriodicity_ms10;    
            }else { 
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%s\" for dl_UL_TransmissionPeriodicity choice: minusinfinity,ms0p5,ms0p625,ms1,ms1p25,ms2,ms2p5,ms5,ms10 !\n",
                           RC.config_file_name, i, dl_UL_TransmissionPeriodicity);
            }

            NRRRC_CONFIGURATION_REQ (msg_p).nrofDownlinkSlots[j] = nrofDownlinkSlots;
            if ((nrofDownlinkSlots < 0) || (nrofDownlinkSlots > 320)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for nrofDownlinkSlots choice: 0..320 !\n",
                           RC.config_file_name, i, nrofDownlinkSlots);
            }

            NRRRC_CONFIGURATION_REQ (msg_p).nrofDownlinkSymbols[j] = nrofDownlinkSymbols;
            if ((nrofDownlinkSymbols < 0) || (nrofDownlinkSymbols > 13)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for nrofDownlinkSymbols choice: 0..13 !\n",
                           RC.config_file_name, i, nrofDownlinkSymbols);
            }

            NRRRC_CONFIGURATION_REQ (msg_p).nrofUplinkSlots[j] = nrofUplinkSlots;
            if ((nrofUplinkSlots < 0) || (nrofUplinkSlots > 320)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for nrofUplinkSlots choice: 0..320 !\n",
                           RC.config_file_name, i, nrofUplinkSlots);
            }
 
            NRRRC_CONFIGURATION_REQ (msg_p).nrofUplinkSymbols[j] = nrofUplinkSymbols;
            if ((nrofUplinkSymbols < 0) || (nrofUplinkSymbols > 13)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for nrofUplinkSymbols choice: 0..13 !\n",
                           RC.config_file_name, i, nrofUplinkSymbols);
            }

            /////////////////////////////////NR RACH-ConfigCommon///////////////////////////

            NRRRC_CONFIGURATION_REQ (msg_p).rach_totalNumberOfRA_Preambles[j] = rach_totalNumberOfRA_Preambles;
            if ((rach_totalNumberOfRA_Preambles <1) || (rach_totalNumberOfRA_Preambles>63)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for rach_numberOfRA_Preambles choice: 1..63 !\n",
                           RC.config_file_name, i, rach_totalNumberOfRA_Preambles);
            }

            if (strcmp(rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_choice,"oneEighth")==0) {
              
              NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_choice[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB_PR_oneEighth;
              switch (rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneEighth){
                case 4:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneEighth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneEighth_n4;
                  break;
                case 8:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneEighth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneEighth_n8;
                  break;
                case 12:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneEighth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneEighth_n12;
                  break;
                case 16:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneEighth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneEighth_n16;
                  break;
                case 20:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneEighth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneEighth_n20;
                  break;
                case 24:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneEighth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneEighth_n24;
                  break;
                case 28:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneEighth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneEighth_n28;
                  break;
                case 32:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneEighth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneEighth_n32;
                  break;
                case 36:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneEighth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneEighth_n36;
                  break;
                case 40:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneEighth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneEighth_n40;
                  break;
                case 44:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneEighth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneEighth_n44;
                  break;
                case 48:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneEighth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneEighth_n48;
                  break;
                case 52:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneEighth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneEighth_n52;
                  break;
                case 56:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneEighth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneEighth_n56;
                  break;
                case 60:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneEighth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneEighth_n60;
                  break;
                case 64:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneEighth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneEighth_n64;
                  break;
                default:
                  AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneEighth choice: 4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64!\n",
                               RC.config_file_name, i, rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneEighth);
                  break;
              }//End oneEighth

            }else if (strcmp(rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_choice,"oneFourth")==0) {
              
              NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_choice[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB_PR_oneFourth;
              switch (rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneFourth){
                case 4:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneFourth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneFourth_n4;
                  break;
                case 8:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneFourth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneFourth_n8;
                  break;
                case 12:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneFourth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneFourth_n12;
                  break;
                case 16:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneFourth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneFourth_n16;
                  break;
                case 20:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneFourth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneFourth_n20;
                  break;
                case 24:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneFourth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneFourth_n24;
                  break;
                case 28:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneFourth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneFourth_n28;
                  break;
                case 32:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneFourth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneFourth_n32;
                  break;
                case 36:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneFourth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneFourth_n36;
                  break;
                case 40:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneFourth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneFourth_n40;
                  break;
                case 44:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneFourth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneFourth_n44;
                  break;
                case 48:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneFourth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneFourth_n48;
                  break;
                case 52:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneFourth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneFourth_n52;
                  break;
                case 56:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneFourth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneFourth_n56;
                  break;
                case 60:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneFourth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneFourth_n60;
                  break;
                case 64:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneFourth[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneFourth_n64;
                  break;
                default:
                  AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneFourth choice: 4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64!\n",
                               RC.config_file_name, i, rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneFourth);
                  break;
              }//End oneFourth

            }else if (strcmp(rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_choice,"oneHalf")==0) {
              
              NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_choice[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB_PR_oneHalf;
              switch (rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneHalf){
                case 4:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneHalf[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneHalf_n4;
                  break;
                case 8:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneHalf[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneHalf_n8;
                  break;
                case 12:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneHalf[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneHalf_n12;
                  break;
                case 16:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneHalf[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneHalf_n16;
                  break;
                case 20:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneHalf[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneHalf_n20;
                  break;
                case 24:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneHalf[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneHalf_n24;
                  break;
                case 28:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneHalf[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneHalf_n28;
                  break;
                case 32:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneHalf[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneHalf_n32;
                  break;
                case 36:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneHalf[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneHalf_n36;
                  break;
                case 40:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneHalf[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneHalf_n40;
                  break;
                case 44:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneHalf[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneHalf_n44;
                  break;
                case 48:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneHalf[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneHalf_n48;
                  break;
                case 52:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneHalf[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneHalf_n52;
                  break;
                case 56:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneHalf[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneHalf_n56;
                  break;
                case 60:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneHalf[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneHalf_n60;
                  break;
                case 64:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneHalf[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__oneHalf_n64;
                  break;
                default:
                  AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneHalf choice: 4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64!\n",
                               RC.config_file_name, i, rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_oneHalf);
                  break;
              }//End oneHalf

            }else if (strcmp(rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_choice,"one")==0) {
              
              NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_choice[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB_PR_one;
              switch (rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_one){
                case 4:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_one[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n4;
                  break;
                case 8:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_one[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n8;
                  break;
                case 12:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_one[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n12;
                  break;
                case 16:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_one[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n16;
                  break;
                case 20:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_one[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n20;
                  break;
                case 24:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_one[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n24;
                  break;
                case 28:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_one[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n28;
                  break;
                case 32:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_one[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n32;
                  break;
                case 36:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_one[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n36;
                  break;
                case 40:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_one[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n40;
                  break;
                case 44:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_one[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n44;
                  break;
                case 48:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_one[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n48;
                  break;
                case 52:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_one[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n52;
                  break;
                case 56:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_one[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n56;
                  break;
                case 60:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_one[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n60;
                  break;
                case 64:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_one[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__one_n64;
                  break;
                default:
                  AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_one choice: 4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64!\n",
                               RC.config_file_name, i, rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_one);
                  break;
              }//End one

            }else if (strcmp(rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_choice,"two")==0) {
              
              NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_choice[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB_PR_two;
              switch (rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_one){
                case 4:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_two[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__two_n4;
                  break;
                case 8:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_two[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__two_n8;
                  break;
                case 12:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_two[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__two_n12;
                  break;
                case 16:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_two[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__two_n16;
                  break;
                case 20:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_two[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__two_n20;
                  break;
                case 24:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_two[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__two_n24;
                  break;
                case 28:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_two[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__two_n28;
                  break;
                case 32:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_two[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB__two_n32;
                  break;
                default:
                  AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_two choice: 4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64!\n",
                               RC.config_file_name, i, rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_two);
                  break;
              }//End two

            }else if (strcmp(rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_choice,"four")==0) {
              
              NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_choice[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB_PR_four;
              NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_four[j] = rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_four;
              if ((rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_four < 1) || (rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_four > 16)){
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_four choice: 1..16 !\n",
                             RC.config_file_name, i, rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_four);
              }//End four

            }else if (strcmp(rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_choice,"eight")==0) {
              
              NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_choice[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB_PR_eight;
              NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_eight[j] = rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_eight;
              if ((rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_eight < 1) || (rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_eight > 8)){
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_eight choice: 1..8 !\n",
                             RC.config_file_name, i, rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_eight);
              }//End eight 

            }else if (strcmp(rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_choice,"sixteen")==0) {
              
              NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_choice[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB_PR_sixteen;    
              NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_sixteen[j] = rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_sixteen;
              if ((rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_sixteen < 1) || (rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_sixteen > 4)){
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_sixteen choice: 1..4 !\n",
                             RC.config_file_name, i, rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_sixteen);
              }//End sixteen

            }else if (strcmp(rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_choice,"NOTHING")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_choice[j] = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB_PR_NOTHING;    
            }else { 
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%s\" for rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_choice: oneEighth,oneFourth,oneHalf,one,two,four,eight,sixteen !\n",
                           RC.config_file_name, i, rach_ssb_perRACH_OccasionAndCB_PreamblesPerSSB_choice);
            }

            if (strcmp(rach_groupBconfigured , "ENABLE") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).rach_groupBconfigured [j] = TRUE;

              switch (rach_ra_Msg3SizeGroupA) {
                case 56:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ra_Msg3SizeGroupA[j] = RACH_ConfigCommon__groupBconfigured__ra_Msg3SizeGroupA_b56;
                  break;
                case 144:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ra_Msg3SizeGroupA[j] = RACH_ConfigCommon__groupBconfigured__ra_Msg3SizeGroupA_b144;
                  break;
                case 208:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ra_Msg3SizeGroupA[j] = RACH_ConfigCommon__groupBconfigured__ra_Msg3SizeGroupA_b208;
                  break;
                case 256:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ra_Msg3SizeGroupA[j] = RACH_ConfigCommon__groupBconfigured__ra_Msg3SizeGroupA_b256;
                  break;
                case 282:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ra_Msg3SizeGroupA[j] = RACH_ConfigCommon__groupBconfigured__ra_Msg3SizeGroupA_b282;
                  break;
                case 480:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ra_Msg3SizeGroupA[j] = RACH_ConfigCommon__groupBconfigured__ra_Msg3SizeGroupA_b480;
                  break;
                case 640:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ra_Msg3SizeGroupA[j] = RACH_ConfigCommon__groupBconfigured__ra_Msg3SizeGroupA_b640;
                  break;
                case 800:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ra_Msg3SizeGroupA[j] = RACH_ConfigCommon__groupBconfigured__ra_Msg3SizeGroupA_b800;
                  break;
                case 1000:
                  NRRRC_CONFIGURATION_REQ (msg_p).rach_ra_Msg3SizeGroupA[j] = RACH_ConfigCommon__groupBconfigured__ra_Msg3SizeGroupA_b1000;
                  break;
                default:
                  AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for rach_ra_Msg3SizeGroupA choice: 56,144,208,256,282,480,640,800,1000!\n",
                               RC.config_file_name, i, rach_ra_Msg3SizeGroupA);
                  break;
              }// End switch (rach_ra_Msg3SizeGroupA)

              if (strcmp(rach_messagePowerOffsetGroupB,"minusinfinity")==0) {
                RRC_CONFIGURATION_REQ (msg_p).rach_messagePowerOffsetGroupB[j] = RACH_ConfigCommon__groupBconfigured__messagePowerOffsetGroupB_minusinfinity;
              }else if (strcmp(rach_messagePowerOffsetGroupB,"dB0")==0) {
                RRC_CONFIGURATION_REQ (msg_p).rach_messagePowerOffsetGroupB[j] = RACH_ConfigCommon__groupBconfigured__messagePowerOffsetGroupB_dB0;
              }else if (strcmp(rach_messagePowerOffsetGroupB,"dB5")==0) {
                RRC_CONFIGURATION_REQ (msg_p).rach_messagePowerOffsetGroupB[j] = RACH_ConfigCommon__groupBconfigured__messagePowerOffsetGroupB_dB5;
              }else if (strcmp(rach_messagePowerOffsetGroupB,"dB8")==0) {
                RRC_CONFIGURATION_REQ (msg_p).rach_messagePowerOffsetGroupB[j] = RACH_ConfigCommon__groupBconfigured__messagePowerOffsetGroupB_dB8;
              }else if (strcmp(rach_messagePowerOffsetGroupB,"dB10")==0) {
                RRC_CONFIGURATION_REQ (msg_p).rach_messagePowerOffsetGroupB[j] = RACH_ConfigCommon__groupBconfigured__messagePowerOffsetGroupB_dB10;
              }else if (strcmp(rach_messagePowerOffsetGroupB,"dB12")==0) {
                RRC_CONFIGURATION_REQ (msg_p).rach_messagePowerOffsetGroupB[j] = RACH_ConfigCommon__groupBconfigured__messagePowerOffsetGroupB_dB12;
              }else if (strcmp(rach_messagePowerOffsetGroupB,"dB15")==0) {
                RRC_CONFIGURATION_REQ (msg_p).rach_messagePowerOffsetGroupB[j] = RACH_ConfigCommon__groupBconfigured__messagePowerOffsetGroupB_dB15;
              }else if (strcmp(rach_messagePowerOffsetGroupB,"dB18")==0) {
                RRC_CONFIGURATION_REQ (msg_p).rach_messagePowerOffsetGroupB[j] = RACH_ConfigCommon__groupBconfigured__messagePowerOffsetGroupB_dB18;
              }else{
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%s\" for rach_messagePowerOffsetGroupB choice: minusinfinity,dB0,dB5,dB8,dB10,dB12,dB15,dB18!\n",
                             RC.config_file_name, i, rach_messagePowerOffsetGroupB);
              }// End if (strcmp(rach_messagePowerOffsetGroupB,"minusinfinity")==0)

              NRRRC_CONFIGURATION_REQ (msg_p).rach_numberOfRA_PreamblesGroupA[j] = rach_numberOfRA_PreamblesGroupA;
              if ((rach_numberOfRA_PreamblesGroupA <1) || (rach_numberOfRA_PreamblesGroupA>64)){
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for rach_numberOfRA_PreamblesGroupA choice: 1..63 !\n",
                             RC.config_file_name, i, rach_numberOfRA_PreamblesGroupA);
              }

            }// End if (strcmp(rach_groupBconfigured , "ENABLE") == 0) 

            switch (rach_ra_ContentionResolutionTimer) {
              case 8:
                NRRRC_CONFIGURATION_REQ (msg_p).rach_ra_ContentionResolutionTimer[j] = RACH_ConfigCommon__ra_ContentionResolutionTimer_sf8;
                break;
              case 16:
                NRRRC_CONFIGURATION_REQ (msg_p).rach_ra_ContentionResolutionTimer[j] = RACH_ConfigCommon__ra_ContentionResolutionTimer_sf16;
                break;
              case 24:
                NRRRC_CONFIGURATION_REQ (msg_p).rach_ra_ContentionResolutionTimer[j] = RACH_ConfigCommon__ra_ContentionResolutionTimer_sf24;
                break;
              case 32:
                NRRRC_CONFIGURATION_REQ (msg_p).rach_ra_ContentionResolutionTimer[j] = RACH_ConfigCommon__ra_ContentionResolutionTimer_sf32;
                break;
              case 40:
                NRRRC_CONFIGURATION_REQ (msg_p).rach_ra_ContentionResolutionTimer[j] = RACH_ConfigCommon__ra_ContentionResolutionTimer_sf40;
                break;
              case 48:
                NRRRC_CONFIGURATION_REQ (msg_p).rach_ra_ContentionResolutionTimer[j] = RACH_ConfigCommon__ra_ContentionResolutionTimer_sf48;
                break;
              case 56:
                NRRRC_CONFIGURATION_REQ (msg_p).rach_ra_ContentionResolutionTimer[j] = RACH_ConfigCommon__ra_ContentionResolutionTimer_sf56;
                break;
              case 64:
                NRRRC_CONFIGURATION_REQ (msg_p).rach_ra_ContentionResolutionTimer[j] = RACH_ConfigCommon__ra_ContentionResolutionTimer_sf64;
                break;
              default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for rach_ra_ContentionResolutionTimer choice: 8,16,24,32,40,48,56,64!\n",
                             RC.config_file_name, i, rach_ra_ContentionResolutionTimer);
                break;
            }// End switch (rach_ra_ContentionResolutionTimer)

            
            NRRRC_CONFIGURATION_REQ (msg_p).rsrp_ThresholdSSB[j] = rsrp_ThresholdSSB;
            if ((rsrp_ThresholdSSB <0) || (rsrp_ThresholdSSB>124)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for rsrp_ThresholdSSB choice: 0..124 !\n",
                           RC.config_file_name, i, rsrp_ThresholdSSB);
            }

            NRRRC_CONFIGURATION_REQ (msg_p).rsrp_ThresholdSSB_SUL[j] = rsrp_ThresholdSSB_SUL;
            if ((rsrp_ThresholdSSB_SUL <0) || (rsrp_ThresholdSSB_SUL>124)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for rsrp_ThresholdSSB_SUL choice: 0..124 !\n",
                           RC.config_file_name, i, rsrp_ThresholdSSB_SUL);
            }

            if (strcmp(prach_RootSequenceIndex_choice , "l839") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).prach_RootSequenceIndex_choice[j] = RACH_ConfigCommon__prach_RootSequenceIndex_PR_l839;
              NRRRC_CONFIGURATION_REQ (msg_p).prach_RootSequenceIndex_l839[j] = prach_RootSequenceIndex_l839;              
            }else if (strcmp(prach_RootSequenceIndex_choice , "l139") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).prach_RootSequenceIndex_choice[j] = RACH_ConfigCommon__prach_RootSequenceIndex_PR_l139;
              NRRRC_CONFIGURATION_REQ (msg_p).prach_RootSequenceIndex_l139[j] = prach_RootSequenceIndex_l139;
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for prach_RootSequenceIndex !\n",
                           RC.config_file_name, i, prach_RootSequenceIndex);
            }

            if (strcmp(prach_msg1_SubcarrierSpacing,"kHz15")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).prach_msg1_SubcarrierSpacing[j] = SubcarrierSpacing_kHz15;
            }else if (strcmp(prach_msg1_SubcarrierSpacing,"kHz30")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).prach_msg1_SubcarrierSpacing[j] = SubcarrierSpacing_kHz30;
            }else if (strcmp(prach_msg1_SubcarrierSpacing,"kHz60")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).prach_msg1_SubcarrierSpacing[j] = SubcarrierSpacing_kHz60;
            }else if (strcmp(prach_msg1_SubcarrierSpacing,"kHz120")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).prach_msg1_SubcarrierSpacing[j] = SubcarrierSpacing_kHz120;
            }else if (strcmp(prach_msg1_SubcarrierSpacing,"kHz240")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).prach_msg1_SubcarrierSpacing[j] = SubcarrierSpacing_kHz240;
            }else { 
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%s\" for prach_msg1_SubcarrierSpacing choice: minusinfinity,kHz15,kHz30,kHz60,kHz120,kHz240!\n",
                           RC.config_file_name, i, prach_msg1_SubcarrierSpacing);
            }

            if (strcmp(restrictedSetConfig , "unrestrictedSet") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).restrictedSetConfig[j] = RACH_ConfigCommon__restrictedSetConfig_unrestrictedSet;                    
            }else if (strcmp(restrictedSetConfig , "restrictedSetTypeA") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).restrictedSetConfig[j] = RACH_ConfigCommon__restrictedSetConfig_restrictedSetTypeA;
            }else if (strcmp(restrictedSetConfig , "restrictedSetTypeB") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).restrictedSetConfig[j] = RACH_ConfigCommon__restrictedSetConfig_restrictedSetTypeB;
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for restrictedSetConfig !\n",
                           RC.config_file_name, i, restrictedSetConfig);
            }

            if (strcmp(msg3_transformPrecoding , "ENABLE") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).msg3_transformPrecoding[j] = TRUE;
            }

            ////////////////////////////////NR RACH-ConfigGeneric//////////////////////////////
            NRRRC_CONFIGURATION_REQ (msg_p).prach_ConfigurationIndex[j] = prach_ConfigurationIndex;
            if ((prach_ConfigurationIndex <0) || (prach_ConfigurationIndex>255)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for prach_ConfigurationIndex choice: 0..255 !\n",
                           RC.config_file_name, i, prach_ConfigurationIndex);
            }

            if (strcmp(prach_msg1_FDM , "one") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).prach_msg1_FDM[j] = RACH_ConfigGeneric__msg1_FDM_one;                    
            }else if (strcmp(prach_msg1_FDM , "two") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).prach_msg1_FDM[j] = RACH_ConfigGeneric__msg1_FDM_two;
            }else if (strcmp(prach_msg1_FDM , "four") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).prach_msg1_FDM[j] = RACH_ConfigGeneric__msg1_FDM_four;
            }else if (strcmp(prach_msg1_FDM , "eight") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).prach_msg1_FDM[j] = RACH_ConfigGeneric__msg1_FDM_eight;
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for prach_msg1_FDM !\n",
                           RC.config_file_name, i, prach_msg1_FDM);
            }            
            
            NRRRC_CONFIGURATION_REQ (msg_p).prach_msg1_FrequencyStart[j] = prach_msg1_FrequencyStart;
            if ((prach_msg1_FrequencyStart <0) || (prach_msg1_FrequencyStart>274)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for prach_msg1_FrequencyStart choice: 0..274 !\n",
                           RC.config_file_name, i, prach_msg1_FrequencyStart);
            }           

            NRRRC_CONFIGURATION_REQ (msg_p).zeroCorrelationZoneConfig[j] = zeroCorrelationZoneConfig;
            if ((zeroCorrelationZoneConfig <0) || (zeroCorrelationZoneConfig>15)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for zeroCorrelationZoneConfig choice: 0..15 !\n",
                           RC.config_file_name, i, zeroCorrelationZoneConfig);
            }

            NRRRC_CONFIGURATION_REQ (msg_p).preambleReceivedTargetPower[j] = preambleReceivedTargetPower;
            if ((preambleReceivedTargetPower <-200) || (preambleReceivedTargetPower>-74)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for preambleReceivedTargetPower choice: -200..-74 !\n",
                           RC.config_file_name, i, preambleReceivedTargetPower);
            }

            switch (preambleTransMax) {
              case 3:
                NRRRC_CONFIGURATION_REQ (msg_p).preambleTransMax[j] =  RACH_ConfigGeneric__preambleTransMax_n3;
                break;
              case 4:
                NRRRC_CONFIGURATION_REQ (msg_p).preambleTransMax[j] =  RACH_ConfigGeneric__preambleTransMax_n4;
                break;
              case 5:
                NRRRC_CONFIGURATION_REQ (msg_p).preambleTransMax[j] =  RACH_ConfigGeneric__preambleTransMax_n5;
                break;
              case 6:
                NRRRC_CONFIGURATION_REQ (msg_p).preambleTransMax[j] =  RACH_ConfigGeneric__preambleTransMax_n6;
                break;
              case 7:
                NRRRC_CONFIGURATION_REQ (msg_p).preambleTransMax[j] =  RACH_ConfigGeneric__preambleTransMax_n7;
                break;
              case 8:
                NRRRC_CONFIGURATION_REQ (msg_p).preambleTransMax[j] =  RACH_ConfigGeneric__preambleTransMax_n8;
                break;
              case 10:
                NRRRC_CONFIGURATION_REQ (msg_p).preambleTransMax[j] =  RACH_ConfigGeneric__preambleTransMax_n10;
                break;
              case 20:
                NRRRC_CONFIGURATION_REQ (msg_p).preambleTransMax[j] =  RACH_ConfigGeneric__preambleTransMax_n20;
                break;
              case 50:
                NRRRC_CONFIGURATION_REQ (msg_p).preambleTransMax[j] =  RACH_ConfigGeneric__preambleTransMax_n50;
                break;
              case 100:
                NRRRC_CONFIGURATION_REQ (msg_p).preambleTransMax[j] =  RACH_ConfigGeneric__preambleTransMax_n100;
                break;
              case 200:
                NRRRC_CONFIGURATION_REQ (msg_p).preambleTransMax[j] =  RACH_ConfigGeneric__preambleTransMax_n200;
                break;
               default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for preambleTransMax choice: 3,4,5,6,7,8,10,20,50,100,200 !\n",
                             RC.config_file_name, i, preambleTransMax);
                break;
            }            

            if (strcmp(powerRampingStep , "dB0") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).powerRampingStep[j] = RACH_ConfigGeneric__powerRampingStep_dB0;                    
            }else if (strcmp(powerRampingStep , "dB2") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).powerRampingStep[j] = RACH_ConfigGeneric__powerRampingStep_dB2;
            }else if (strcmp(powerRampingStep , "dB4") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).powerRampingStep[j] = RACH_ConfigGeneric__powerRampingStep_dB4;
            }else if (strcmp(powerRampingStep , "dB6") == 0) {
              NRRRC_CONFIGURATION_REQ (msg_p).powerRampingStep[j] = RACH_ConfigGeneric__powerRampingStep_dB6;
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for powerRampingStep !\n",
                           RC.config_file_name, i, powerRampingStep);
            }

            switch (ra_ResponseWindow) {
              case 1:
                NRRRC_CONFIGURATION_REQ (msg_p).ra_ResponseWindow[j] =  RACH_ConfigGeneric__ra_ResponseWindow_sl1;
                break;
              case 2:
                NRRRC_CONFIGURATION_REQ (msg_p).ra_ResponseWindow[j] =  RACH_ConfigGeneric__ra_ResponseWindow_sl2;
                break;
              case 4:
                NRRRC_CONFIGURATION_REQ (msg_p).ra_ResponseWindow[j] =  RACH_ConfigGeneric__ra_ResponseWindow_sl4;
                break;
              case 8:
                NRRRC_CONFIGURATION_REQ (msg_p).ra_ResponseWindow[j] =  RACH_ConfigGeneric__ra_ResponseWindow_sl8;
                break;
              case 10:
                NRRRC_CONFIGURATION_REQ (msg_p).ra_ResponseWindow[j] =  RACH_ConfigGeneric__ra_ResponseWindow_sl10;
                break;
              case 20:
                NRRRC_CONFIGURATION_REQ (msg_p).ra_ResponseWindow[j] =  RACH_ConfigGeneric__ra_ResponseWindow_sl20;
                break;
              case 40:
                NRRRC_CONFIGURATION_REQ (msg_p).ra_ResponseWindow[j] =  RACH_ConfigGeneric__ra_ResponseWindow_sl40;
                break;
              case 80:
                NRRRC_CONFIGURATION_REQ (msg_p).ra_ResponseWindow[j] =  RACH_ConfigGeneric__ra_ResponseWindow_sl80;
                break;
               default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for ra_ResponseWindow choice: 1,2,4,8,10,20,40,80 !\n",
                             RC.config_file_name, i, ra_ResponseWindow);
                break;
            }  

            /////////////////////////////////NR PUSCH-ConfigCommon///////////////////////////
            if (strcmp(groupHoppingEnabledTransformPrecoding , "ENABLE") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).groupHoppingEnabledTransformPrecoding[j] =  TRUE;
            }

            NRRRC_CONFIGURATION_REQ (msg_p).msg3_DeltaPreamble[j] = msg3_DeltaPreamble;
            if ((msg3_DeltaPreamble <-1) || (msg3_DeltaPreamble>6)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for msg3_DeltaPreamble choice: -1..6 !\n",
                           RC.config_file_name, i, msg3_DeltaPreamble);
            }

            NRRRC_CONFIGURATION_REQ (msg_p).p0_NominalWithGrant[j] = p0_NominalWithGrant;
            if ((p0_NominalWithGrant <-202) || (p0_NominalWithGrant>24)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for p0_NominalWithGrant choice: -202..24 !\n",
                           RC.config_file_name, i, p0_NominalWithGrant);
            }

            /////////////////////////////////NR PUSCH-TimeDomainResourceAllocation///////////////////////////
            NRRRC_CONFIGURATION_REQ (msg_p).PUSCH_TimeDomainResourceAllocation_k2[j] = PUSCH_TimeDomainResourceAllocation_k2;
            if ((PUSCH_TimeDomainResourceAllocation_k2 <0) || (PUSCH_TimeDomainResourceAllocation_k2>7)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for PUSCH_TimeDomainResourceAllocation_k2 choice: 0..7 !\n",
                           RC.config_file_name, i, PUSCH_TimeDomainResourceAllocation_k2);
            }

            if (strcmp(PUSCH_TimeDomainResourceAllocation_mappingType , "typeA") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).PUSCH_TimeDomainResourceAllocation_mappingType[j] =  PUSCH_TimeDomainResourceAllocation__mappingType_typeA;
            }else if (strcmp(PUSCH_TimeDomainResourceAllocation_mappingType , "typeB") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).PUSCH_TimeDomainResourceAllocation_mappingType[j] =  PUSCH_TimeDomainResourceAllocation__mappingType_typeB;
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for PUSCH_TimeDomainResourceAllocation_mappingType !\n",
                           RC.config_file_name, i, PUSCH_TimeDomainResourceAllocation_mappingType);
            }

            /////////////////////////////////NR PUCCH-ConfigCommon///////////////////////////
            if (strcmp(pucch_GroupHopping , "neither") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).pucch_GroupHopping[j] =  PUCCH_ConfigCommon__pucch_GroupHopping_neither;
            }else if (strcmp(pucch_GroupHopping , "enable") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).pucch_GroupHopping[j] =  PUCCH_ConfigCommon__pucch_GroupHopping_enable;
            }else if (strcmp(pucch_GroupHopping , "disable") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).pucch_GroupHopping[j] =  PUCCH_ConfigCommon__pucch_GroupHopping_disable;
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for pucch_GroupHopping !\n",
                           RC.config_file_name, i, pucch_GroupHopping);
            }

            NRRRC_CONFIGURATION_REQ (msg_p).p0_nominal[j] = p0_nominal;
            if ((p0_nominal <-202) || (p0_nominal>24)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for p0_nominal choice: -202..24 !\n",
                           RC.config_file_name, i, p0_nominal);
            }

            //////////////////////////////////NR PDSCH-TimeDomainResourceAllocation///////////////////////////
            NRRRC_CONFIGURATION_REQ (msg_p).PDSCH_TimeDomainResourceAllocation_k0[j] = PDSCH_TimeDomainResourceAllocation_k0;
            if ((PDSCH_TimeDomainResourceAllocation_k0 <1) || (PDSCH_TimeDomainResourceAllocation_k0>3)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for PDSCH_TimeDomainResourceAllocation_k0 choice: 0..7 !\n",
                           RC.config_file_name, i, PDSCH_TimeDomainResourceAllocation_k0);
            }

            if (strcmp(PDSCH_TimeDomainResourceAllocation_mappingType , "typeA") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).PDSCH_TimeDomainResourceAllocation_mappingType[j] =  PDSCH_TimeDomainResourceAllocation__mappingType_typeA;
            }else if (strcmp(PDSCH_TimeDomainResourceAllocation_mappingType , "typeB") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).PDSCH_TimeDomainResourceAllocation_mappingType[j] =  PDSCH_TimeDomainResourceAllocation__mappingType_typeB;
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for PDSCH_TimeDomainResourceAllocation_mappingType !\n",
                           RC.config_file_name, i, PDSCH_TimeDomainResourceAllocation_mappingType);
            }

            //////////////////////////////////NR RateMatchPattern///////////////////////////
            NRRRC_CONFIGURATION_REQ (msg_p).rateMatchPatternId[j] = rateMatchPatternId;
            if ((rateMatchPatternId <0) || (rateMatchPatternId>3)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for rateMatchPatternId choice: 0..3 !\n",
                           RC.config_file_name, i, rateMatchPatternId);
            }

            if (strcmp(RateMatchPattern_patternType , "NOTHING") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPattern_patternType[j] =  RateMatchPattern__patternType_PR_NOTHING;
            }else if (strcmp(RateMatchPattern_patternType , "bitmaps") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPattern_patternType[j] =  RateMatchPattern__patternType_PR_bitmaps;
            }else if (strcmp(RateMatchPattern_patternType , "controlResourceSet") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPattern_patternType[j] =  RateMatchPattern__patternType_PR_controlResourceSet;
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for RateMatchPattern_patternType !\n",
                           RC.config_file_name, i, RateMatchPattern_patternType);
            }

            if (strcmp(symbolsInResourceBlock , "NOTHING") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).symbolsInResourceBlock[j] =  RateMatchPattern__patternType__bitmaps__symbolsInResourceBlock_PR_NOTHING;
            }else if (strcmp(symbolsInResourceBlock , "oneSlot") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).symbolsInResourceBlock[j] =  RateMatchPattern__patternType__bitmaps__symbolsInResourceBlock_PR_oneSlot;
            }else if (strcmp(symbolsInResourceBlock , "twoSlots") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).symbolsInResourceBlock[j] =  RateMatchPattern__patternType__bitmaps__symbolsInResourceBlock_PR_twoSlots;
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for symbolsInResourceBlock !\n",
                           RC.config_file_name, i, symbolsInResourceBlock);
            }

            switch(periodicityAndPattern){
              case 2:
                NRRRC_CONFIGURATION_REQ (msg_p).periodicityAndPattern[j] =  RateMatchPattern__patternType__bitmaps__periodicityAndPattern_PR_n2;
                break;
              case 4:
                NRRRC_CONFIGURATION_REQ (msg_p).periodicityAndPattern[j] =  RateMatchPattern__patternType__bitmaps__periodicityAndPattern_PR_n4;
                break;
              case 5:
                NRRRC_CONFIGURATION_REQ (msg_p).periodicityAndPattern[j] =  RateMatchPattern__patternType__bitmaps__periodicityAndPattern_PR_n5;
                break;
              case 8:
                NRRRC_CONFIGURATION_REQ (msg_p).periodicityAndPattern[j] =  RateMatchPattern__patternType__bitmaps__periodicityAndPattern_PR_n8;
                break;
              case 10:
                NRRRC_CONFIGURATION_REQ (msg_p).periodicityAndPattern[j] =  RateMatchPattern__patternType__bitmaps__periodicityAndPattern_PR_n10;
                break;
              case 20:
                NRRRC_CONFIGURATION_REQ (msg_p).periodicityAndPattern[j] =  RateMatchPattern__patternType__bitmaps__periodicityAndPattern_PR_n20;
                break;
              case 40:
                NRRRC_CONFIGURATION_REQ (msg_p).periodicityAndPattern[j] =  RateMatchPattern__patternType__bitmaps__periodicityAndPattern_PR_n40;
                break;
              default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for periodicityAndPattern choice: 2,4,5,8,10,20,40 !\n",
                             RC.config_file_name, i, periodicityAndPattern);
                break;
            }

            NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPattern_controlResourceSet[j] = RateMatchPattern_controlResourceSet;
            if ((RateMatchPattern_controlResourceSet <0) || (RateMatchPattern_controlResourceSet>11)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for RateMatchPattern_controlResourceSet choice: 0..3 !\n",
                           RC.config_file_name, i, RateMatchPattern_controlResourceSet);
            }

            if (strcmp(RateMatchPattern_subcarrierSpacing,"kHz15")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPattern_subcarrierSpacing[j] = SubcarrierSpacing_kHz15;
            }else if (strcmp(RateMatchPattern_subcarrierSpacing,"kHz30")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPattern_subcarrierSpacing[j] = SubcarrierSpacing_kHz30;
            }else if (strcmp(RateMatchPattern_subcarrierSpacing,"kHz60")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPattern_subcarrierSpacing[j] = SubcarrierSpacing_kHz60;
            }else if (strcmp(RateMatchPattern_subcarrierSpacing,"kHz120")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPattern_subcarrierSpacing[j] = SubcarrierSpacing_kHz120;
            }else if (strcmp(RateMatchPattern_subcarrierSpacing,"kHz240")==0) {
              NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPattern_subcarrierSpacing[j] = SubcarrierSpacing_kHz240;
            }else { 
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%s\" for RateMatchPattern_subcarrierSpacing choice: minusinfinity,kHz15,kHz30,kHz60,kHz120,kHz240!\n",
                           RC.config_file_name, i, RateMatchPattern_subcarrierSpacing);
            }            
            
            if (strcmp(RateMatchPattern_mode , "dynamic") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPattern_mode[j] =  RateMatchPattern__mode_dynamic;
            }else if (strcmp(RateMatchPattern_mode , "semiStatic") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPattern_mode[j] =  RateMatchPattern__mode_semiStatic;
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for RateMatchPattern_mode !\n",
                           RC.config_file_name, i, RateMatchPattern_mode);
            }

            //////////////////////////////////NR PDCCH-ConfigCommon///////////////////////////
            NRRRC_CONFIGURATION_REQ (msg_p).searchSpaceSIB1[j] = searchSpaceSIB1;
            if ((searchSpaceSIB1 <0) || (searchSpaceSIB1>39)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for searchSpaceSIB1 choice: 0..39 !\n",
                           RC.config_file_name, i, searchSpaceSIB1);
            }

            NRRRC_CONFIGURATION_REQ (msg_p).searchSpaceOtherSystemInformation[j] = searchSpaceOtherSystemInformation;
            if ((searchSpaceOtherSystemInformation <0) || (searchSpaceOtherSystemInformation>39)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for searchSpaceOtherSystemInformation choice: 0..39 !\n",
                           RC.config_file_name, i, searchSpaceOtherSystemInformation);
            }            

            NRRRC_CONFIGURATION_REQ (msg_p).pagingSearchSpace[j] = pagingSearchSpace;
            if ((pagingSearchSpace <0) || (pagingSearchSpace>39)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for pagingSearchSpace choice: 0..39 !\n",
                           RC.config_file_name, i, pagingSearchSpace);
            }

            NRRRC_CONFIGURATION_REQ (msg_p).ra_SearchSpace[j] = ra_SearchSpace;
            if ((ra_SearchSpace <0) || (ra_SearchSpace>39)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for ra_SearchSpace choice: 0..39 !\n",
                           RC.config_file_name, i, ra_SearchSpace);
            }

            NRRRC_CONFIGURATION_REQ (msg_p).rach_ra_ControlResourceSet[j] = rach_ra_ControlResourceSet;
            if ((rach_ra_ControlResourceSet <0) || (rach_ra_ControlResourceSet>11)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for rach_ra_ControlResourceSet choice: 0..11 !\n",
                           RC.config_file_name, i, rach_ra_ControlResourceSet);
            }

            //////////////////////////////////NR PDCCH commonControlResourcesSets///////////////////////////
            NRRRC_CONFIGURATION_REQ (msg_p).PDCCH_common_controlResourceSetId[j] = PDCCH_common_controlResourceSetId;
            if ((PDCCH_common_controlResourceSetId <0) || (PDCCH_common_controlResourceSetId>11)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for PDCCH_common_controlResourceSetId choice: 0..11 !\n",
                           RC.config_file_name, i, PDCCH_common_controlResourceSetId);
            }

            NRRRC_CONFIGURATION_REQ (msg_p).PDCCH_common_ControlResourceSet_duration[j] = PDCCH_common_ControlResourceSet_duration;
            if ((PDCCH_common_ControlResourceSet_duration <0) || (PDCCH_common_ControlResourceSet_duration>3)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for PDCCH_common_ControlResourceSet_duration choice: 0..11 !\n",
                           RC.config_file_name, i, PDCCH_common_ControlResourceSet_duration);
            }            
            
            if (strcmp(PDCCH_cce_REG_MappingType , "NOTHING") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).PDCCH_cce_REG_MappingType[j] =  ControlResourceSet__cce_REG_MappingType_PR_NOTHING;
            }else if (strcmp(PDCCH_cce_REG_MappingType , "interleaved") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).PDCCH_cce_REG_MappingType[j] =  ControlResourceSet__cce_REG_MappingType_PR_interleaved;
            }else if (strcmp(PDCCH_cce_REG_MappingType , "nonInterleaved") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).PDCCH_cce_REG_MappingType[j] =  ControlResourceSet__cce_REG_MappingType_PR_nonInterleaved;
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for PDCCH_cce_REG_MappingType !\n",
                           RC.config_file_name, i, PDCCH_cce_REG_MappingType);
            }

            switch(PDCCH_reg_BundleSize){
              case 2:
                NRRRC_CONFIGURATION_REQ (msg_p).PDCCH_reg_BundleSize[j] =  ControlResourceSet__cce_REG_MappingType__interleaved__reg_BundleSize_n2;
                break;
              case 3:
                NRRRC_CONFIGURATION_REQ (msg_p).PDCCH_reg_BundleSize[j] =  ControlResourceSet__cce_REG_MappingType__interleaved__reg_BundleSize_n3;
                break;
              case 6:
                NRRRC_CONFIGURATION_REQ (msg_p).PDCCH_reg_BundleSize[j] =  ControlResourceSet__cce_REG_MappingType__interleaved__reg_BundleSize_n6;
                break;
              default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for PDCCH_reg_BundleSize choice: 2,3,6 !\n",
                             RC.config_file_name, i, PDCCH_reg_BundleSize);
                break;
            }            

            switch(PDCCH_interleaverSize){
              case 2:
                NRRRC_CONFIGURATION_REQ (msg_p).PDCCH_interleaverSize[j] =  ControlResourceSet__cce_REG_MappingType__interleaved__reg_BundleSize_n2;
                break;
              case 3:
                NRRRC_CONFIGURATION_REQ (msg_p).PDCCH_interleaverSize[j] =  ControlResourceSet__cce_REG_MappingType__interleaved__reg_BundleSize_n3;
                break;
              case 6:
                NRRRC_CONFIGURATION_REQ (msg_p).PDCCH_interleaverSize[j] =  ControlResourceSet__cce_REG_MappingType__interleaved__reg_BundleSize_n6;
                break;
              default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for PDCCH_interleaverSize choice: 2,3,6 !\n",
                             RC.config_file_name, i, PDCCH_interleaverSize);
                break;
            }               

            NRRRC_CONFIGURATION_REQ (msg_p).PDCCH_shiftIndex[j] = PDCCH_shiftIndex;
            if ((PDCCH_shiftIndex <0) || (PDCCH_shiftIndex>274)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for PDCCH_shiftIndex choice: 0..274 !\n",
                           RC.config_file_name, i, PDCCH_shiftIndex);
            }

            if (strcmp(PDCCH_precoderGranularity , "sameAsREG-bundle") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).PDCCH_precoderGranularity[j] =  ControlResourceSet__precoderGranularity_sameAsREG_bundle;
            }else if (strcmp(PDCCH_precoderGranularity , "allContiguousRBs") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).PDCCH_precoderGranularity[j] =  ControlResourceSet__precoderGranularity_allContiguousRBs;
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for PDCCH_precoderGranularity !\n",
                           RC.config_file_name, i, PDCCH_precoderGranularity);
            }            

            NRRRC_CONFIGURATION_REQ (msg_p).PDCCH_TCI_StateId[j] = PDCCH_TCI_StateId;
            if ((PDCCH_TCI_StateId <0) || (PDCCH_TCI_StateId>63)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for PDCCH_TCI_StateId choice: 0..63 !\n",
                           RC.config_file_name, i, PDCCH_TCI_StateId);
            }

            if (strcmp(tci_PresentInDCI , "ENABLE") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).tci_PresentInDCI[j] = TRUE;
            }

            //////////////////////////////////NR PDCCH commonSearchSpaces///////////////////////////
            NRRRC_CONFIGURATION_REQ (msg_p).SearchSpaceId[j] = SearchSpaceId;
            if ((SearchSpaceId <0) || (SearchSpaceId>39)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for SearchSpaceId choice: 0..39 !\n",
                           RC.config_file_name, i, SearchSpaceId);
            }            

            NRRRC_CONFIGURATION_REQ (msg_p).commonSearchSpaces_controlResourceSetId[j] = commonSearchSpaces_controlResourceSetId;
            if ((commonSearchSpaces_controlResourceSetId <0) || (commonSearchSpaces_controlResourceSetId>11)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for commonSearchSpaces_controlResourceSetId choice: 0..11 !\n",
                           RC.config_file_name, i, commonSearchSpaces_controlResourceSetId);
            }

            if (strcmp(SearchSpace_monitoringSlotPeriodicityAndOffset_choice , "sl1") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_monitoringSlotPeriodicityAndOffset_choice[j] =  SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl1;
              
              NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_monitoringSlotPeriodicityAndOffset_sl1[j] = NULL;                 

            }else if (strcmp(SearchSpace_monitoringSlotPeriodicityAndOffset_choice , "sl2") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_monitoringSlotPeriodicityAndOffset_choice[j] =  SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl2;
              
              NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_monitoringSlotPeriodicityAndOffset_sl2[j] = SearchSpace_monitoringSlotPeriodicityAndOffset_sl2;
              if ((SearchSpace_monitoringSlotPeriodicityAndOffset_sl2 <0) || (SearchSpace_monitoringSlotPeriodicityAndOffset_sl2>1)){
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for SearchSpace_monitoringSlotPeriodicityAndOffset_sl2 choice: 0..1 !\n",
                             RC.config_file_name, i, SearchSpace_monitoringSlotPeriodicityAndOffset_sl2);
              }   

            }else if (strcmp(SearchSpace_monitoringSlotPeriodicityAndOffset_choice , "sl4") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_monitoringSlotPeriodicityAndOffset_choice[j] =  SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl4;
            
              NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_monitoringSlotPeriodicityAndOffset_sl4[j] = SearchSpace_monitoringSlotPeriodicityAndOffset_sl4;
              if ((SearchSpace_monitoringSlotPeriodicityAndOffset_sl4 <0) || (SearchSpace_monitoringSlotPeriodicityAndOffset_sl4>3)){
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for SearchSpace_monitoringSlotPeriodicityAndOffset_sl4 choice: 0..3 !\n",
                             RC.config_file_name, i, SearchSpace_monitoringSlotPeriodicityAndOffset_sl4);
              }                 

            }else if (strcmp(SearchSpace_monitoringSlotPeriodicityAndOffset_choice , "sl5") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_monitoringSlotPeriodicityAndOffset_choice[j] =  SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl5;
            
              NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_monitoringSlotPeriodicityAndOffset_sl5[j] = SearchSpace_monitoringSlotPeriodicityAndOffset_sl5;
              if ((SearchSpace_monitoringSlotPeriodicityAndOffset_sl5 <0) || (SearchSpace_monitoringSlotPeriodicityAndOffset_sl5>4)){
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for SearchSpace_monitoringSlotPeriodicityAndOffset_sl5 choice: 0..4 !\n",
                             RC.config_file_name, i, SearchSpace_monitoringSlotPeriodicityAndOffset_sl5);
              }   

            }else if (strcmp(SearchSpace_monitoringSlotPeriodicityAndOffset_choice , "sl8") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_monitoringSlotPeriodicityAndOffset_choice[j] =  SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl8;
            
              NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_monitoringSlotPeriodicityAndOffset_sl8[j] = SearchSpace_monitoringSlotPeriodicityAndOffset_sl8;
              if ((SearchSpace_monitoringSlotPeriodicityAndOffset_sl8 <0) || (SearchSpace_monitoringSlotPeriodicityAndOffset_sl8>7)){
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for SearchSpace_monitoringSlotPeriodicityAndOffset_sl8 choice: 0..7 !\n",
                             RC.config_file_name, i, SearchSpace_monitoringSlotPeriodicityAndOffset_sl8);
              }   

            }else if (strcmp(SearchSpace_monitoringSlotPeriodicityAndOffset_choice , "sl10") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_monitoringSlotPeriodicityAndOffset_choice[j] =  SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl10;
            
              NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_monitoringSlotPeriodicityAndOffset_sl10[j] = SearchSpace_monitoringSlotPeriodicityAndOffset_sl10;
              if ((SearchSpace_monitoringSlotPeriodicityAndOffset_sl10 <0) || (SearchSpace_monitoringSlotPeriodicityAndOffset_sl10>9)){
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for SearchSpace_monitoringSlotPeriodicityAndOffset_sl10 choice: 0..9 !\n",
                             RC.config_file_name, i, SearchSpace_monitoringSlotPeriodicityAndOffset_sl10);
              } 

            }else if (strcmp(SearchSpace_monitoringSlotPeriodicityAndOffset_choice , "sl16") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_monitoringSlotPeriodicityAndOffset_choice[j] =  SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl16;
            
              NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_monitoringSlotPeriodicityAndOffset_sl16[j] = SearchSpace_monitoringSlotPeriodicityAndOffset_sl16;
              if ((SearchSpace_monitoringSlotPeriodicityAndOffset_sl16 <0) || (SearchSpace_monitoringSlotPeriodicityAndOffset_sl16>15)){
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for SearchSpace_monitoringSlotPeriodicityAndOffset_sl16 choice: 0..15 !\n",
                             RC.config_file_name, i, SearchSpace_monitoringSlotPeriodicityAndOffset_sl16);
              } 

            }else if (strcmp(SearchSpace_monitoringSlotPeriodicityAndOffset_choice , "sl20") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_monitoringSlotPeriodicityAndOffset_choice[j] =  SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl20;
            
              NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_monitoringSlotPeriodicityAndOffset_sl20[j] = SearchSpace_monitoringSlotPeriodicityAndOffset_sl20;
              if ((SearchSpace_monitoringSlotPeriodicityAndOffset_sl20 <0) || (SearchSpace_monitoringSlotPeriodicityAndOffset_sl20>19)){
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for SearchSpace_monitoringSlotPeriodicityAndOffset_sl20 choice: 0..19 !\n",
                             RC.config_file_name, i, SearchSpace_monitoringSlotPeriodicityAndOffset_sl20);
              } 

            }else if (strcmp(SearchSpace_monitoringSlotPeriodicityAndOffset_choice , "UNABLE") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_monitoringSlotPeriodicityAndOffset_choice[j] =  SearchSpace__monitoringSlotPeriodicityAndOffset_PR_NOTHING;
            
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for SearchSpace_monitoringSlotPeriodicityAndOffset_choice !\n",
                           RC.config_file_name, i, SearchSpace_monitoringSlotPeriodicityAndOffset_choice);
            }// End if (strcmp(SearchSpace_monitoringSlotPeriodicityAndOffset_choice , "sl1")

            
            switch(SearchSpace_nrofCandidates_aggregationLevel1){
              case 0:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel1[j] =  SearchSpace__nrofCandidates__aggregationLevel1_n0;
                break;
              case 1:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel1[j] =  SearchSpace__nrofCandidates__aggregationLevel1_n1;
                break;
              case 2:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel1[j] =  SearchSpace__nrofCandidates__aggregationLevel1_n2;
                break;
              case 3:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel1[j] =  SearchSpace__nrofCandidates__aggregationLevel1_n3;
                break;
              case 4:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel1[j] =  SearchSpace__nrofCandidates__aggregationLevel1_n4;
                break;
              case 5:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel1[j] =  SearchSpace__nrofCandidates__aggregationLevel1_n5;
                break;
              case 6:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel1[j] =  SearchSpace__nrofCandidates__aggregationLevel1_n6;
                break;
              case 8:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel1[j] =  SearchSpace__nrofCandidates__aggregationLevel1_n8;
                break;
              default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for SearchSpace_nrofCandidates_aggregationLevel1 choice: 0,1,2,3,4,5,6,8 !\n",
                             RC.config_file_name, i, SearchSpace_nrofCandidates_aggregationLevel1);
                break;
            }  

            switch(SearchSpace_nrofCandidates_aggregationLevel2){
              case 0:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel2[j] =  SearchSpace__nrofCandidates__aggregationLevel2_n0;
                break;
              case 1:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel2[j] =  SearchSpace__nrofCandidates__aggregationLevel2_n1;
                break;
              case 2:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel2[j] =  SearchSpace__nrofCandidates__aggregationLevel2_n2;
                break;
              case 3:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel2[j] =  SearchSpace__nrofCandidates__aggregationLevel2_n3;
                break;
              case 4:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel2[j] =  SearchSpace__nrofCandidates__aggregationLevel2_n4;
                break;
              case 5:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel2[j] =  SearchSpace__nrofCandidates__aggregationLevel2_n5;
                break;
              case 6:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel2[j] =  SearchSpace__nrofCandidates__aggregationLevel2_n6;
                break;
              case 8:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel2[j] =  SearchSpace__nrofCandidates__aggregationLevel2_n8;
                break;
              default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for SearchSpace_nrofCandidates_aggregationLevel2 choice: 0,1,2,3,4,5,6,8 !\n",
                             RC.config_file_name, i, SearchSpace_nrofCandidates_aggregationLevel2);
                break;
            }  

            switch(SearchSpace_nrofCandidates_aggregationLevel4){
              case 0:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel4[j] =  SearchSpace__nrofCandidates__aggregationLevel4_n0;
                break;
              case 1:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel4[j] =  SearchSpace__nrofCandidates__aggregationLevel4_n1;
                break;
              case 2:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel4[j] =  SearchSpace__nrofCandidates__aggregationLevel4_n2;
                break;
              case 3:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel4[j] =  SearchSpace__nrofCandidates__aggregationLevel4_n3;
                break;
              case 4:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel4[j] =  SearchSpace__nrofCandidates__aggregationLevel4_n4;
                break;
              case 5:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel4[j] =  SearchSpace__nrofCandidates__aggregationLevel4_n5;
                break;
              case 6:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel4[j] =  SearchSpace__nrofCandidates__aggregationLevel4_n6;
                break;
              case 8:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel4[j] =  SearchSpace__nrofCandidates__aggregationLevel4_n8;
                break;
              default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for SearchSpace_nrofCandidates_aggregationLevel4 choice: 0,1,2,3,4,5,6,8 !\n",
                             RC.config_file_name, i, SearchSpace_nrofCandidates_aggregationLevel4);
                break;
            }

            switch(SearchSpace_nrofCandidates_aggregationLevel8){
              case 0:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel8[j] =  SearchSpace__nrofCandidates__aggregationLevel8_n0;
                break;
              case 1:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel8[j] =  SearchSpace__nrofCandidates__aggregationLevel8_n1;
                break;
              case 2:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel8[j] =  SearchSpace__nrofCandidates__aggregationLevel8_n2;
                break;
              case 3:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel8[j] =  SearchSpace__nrofCandidates__aggregationLevel8_n3;
                break;
              case 4:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel8[j] =  SearchSpace__nrofCandidates__aggregationLevel8_n4;
                break;
              case 5:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel8[j] =  SearchSpace__nrofCandidates__aggregationLevel8_n5;
                break;
              case 6:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel8[j] =  SearchSpace__nrofCandidates__aggregationLevel8_n6;
                break;
              case 8:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel8[j] =  SearchSpace__nrofCandidates__aggregationLevel8_n8;
                break;
              default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for SearchSpace_nrofCandidates_aggregationLevel8 choice: 0,1,2,3,4,5,6,8 !\n",
                             RC.config_file_name, i, SearchSpace_nrofCandidates_aggregationLevel8);
                break;
            }

            switch(SearchSpace_nrofCandidates_aggregationLevel16){
              case 0:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel16[j] =  SearchSpace__nrofCandidates__aggregationLevel16_n0;
                break;
              case 1:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel16[j] =  SearchSpace__nrofCandidates__aggregationLevel16_n1;
                break;
              case 2:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel16[j] =  SearchSpace__nrofCandidates__aggregationLevel16_n2;
                break;
              case 3:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel16[j] =  SearchSpace__nrofCandidates__aggregationLevel16_n3;
                break;
              case 4:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel16[j] =  SearchSpace__nrofCandidates__aggregationLevel16_n4;
                break;
              case 5:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel16[j] =  SearchSpace__nrofCandidates__aggregationLevel16_n5;
                break;
              case 6:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel16[j] =  SearchSpace__nrofCandidates__aggregationLevel16_n6;
                break;
              case 8:
                NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_nrofCandidates_aggregationLevel16[j] =  SearchSpace__nrofCandidates__aggregationLevel16_n8;
                break;
              default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for SearchSpace_nrofCandidates_aggregationLevel16 choice: 0,1,2,3,4,5,6,8 !\n",
                             RC.config_file_name, i, SearchSpace_nrofCandidates_aggregationLevel16);
                break;
            }

            if (strcmp(SearchSpace_searchSpaceType , "NOTHING") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_searchSpaceType[j] =  SearchSpace__searchSpaceType_PR_NOTHING;
            }else if (strcmp(SearchSpace_searchSpaceType , "common") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_searchSpaceType[j] =  SearchSpace__searchSpaceType_PR_common;
            }else if (strcmp(SearchSpace_searchSpaceType , "ue_Specific") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).SearchSpace_searchSpaceType[j] =  SearchSpace__searchSpaceType_PR_ue_Specific;
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for SearchSpace_searchSpaceType !\n",
                           RC.config_file_name, i, SearchSpace_searchSpaceType);
            }

            switch(Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel1){
              case 1:
                NRRRC_CONFIGURATION_REQ (msg_p).Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel1[j] =  SearchSpace__searchSpaceType__common__dci_Format2_0__nrofCandidates_SFI__aggregationLevel1_n1;
                break;
              case 2:
                NRRRC_CONFIGURATION_REQ (msg_p).Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel1[j] =  SearchSpace__searchSpaceType__common__dci_Format2_0__nrofCandidates_SFI__aggregationLevel1_n2;
                break;
              default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel1 choice: 1,2 !\n",
                             RC.config_file_name, i, Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel1);
                break;
            }

            switch(Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel2){
              case 1:
                NRRRC_CONFIGURATION_REQ (msg_p).Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel2[j] =  SearchSpace__searchSpaceType__common__dci_Format2_0__nrofCandidates_SFI__aggregationLevel2_n1;
                break;
              case 2:
                NRRRC_CONFIGURATION_REQ (msg_p).Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel2[j] =  SearchSpace__searchSpaceType__common__dci_Format2_0__nrofCandidates_SFI__aggregationLevel2_n2;
                break;
              default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel2 choice: 1,2 !\n",
                             RC.config_file_name, i, Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel2);
                break;
            }

            switch(Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel4){
              case 1:
                NRRRC_CONFIGURATION_REQ (msg_p).Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel4[j] =  SearchSpace__searchSpaceType__common__dci_Format2_0__nrofCandidates_SFI__aggregationLevel4_n1;
                break;
              case 2:
                NRRRC_CONFIGURATION_REQ (msg_p).Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel4[j] =  SearchSpace__searchSpaceType__common__dci_Format2_0__nrofCandidates_SFI__aggregationLevel4_n2;
                break;
              default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel4 choice: 1,2 !\n",
                             RC.config_file_name, i, Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel4);
                break;
            }

            switch(Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel8){
              case 1:
                NRRRC_CONFIGURATION_REQ (msg_p).Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel8[j] =  SearchSpace__searchSpaceType__common__dci_Format2_0__nrofCandidates_SFI__aggregationLevel8_n1;
                break;
              case 2:
                NRRRC_CONFIGURATION_REQ (msg_p).Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel8[j] =  SearchSpace__searchSpaceType__common__dci_Format2_0__nrofCandidates_SFI__aggregationLevel8_n2;
                break;
              default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel8 choice: 1,2 !\n",
                             RC.config_file_name, i, Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel8);
                break;
            }

            switch(Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel16){
              case 1:
                NRRRC_CONFIGURATION_REQ (msg_p).Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel16[j] =  SearchSpace__searchSpaceType__common__dci_Format2_0__nrofCandidates_SFI__aggregationLevel16_n1;
                break;
              case 2:
                NRRRC_CONFIGURATION_REQ (msg_p).Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel16[j] =  SearchSpace__searchSpaceType__common__dci_Format2_0__nrofCandidates_SFI__aggregationLevel16_n2;
                break;
              default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel16 choice: 1,2 !\n",
                             RC.config_file_name, i, Common_dci_Format2_0_nrofCandidates_SFI_aggregationLevel16);
                break;
            }

            switch(Common_dci_Format2_3_monitoringPeriodicity){
              case 1:
                NRRRC_CONFIGURATION_REQ (msg_p).Common_dci_Format2_3_monitoringPeriodicity[j] =  SearchSpace__searchSpaceType__common__dci_Format2_3__monitoringPeriodicity_n1;
                break;
              case 2:
                NRRRC_CONFIGURATION_REQ (msg_p).Common_dci_Format2_3_monitoringPeriodicity[j] =  SearchSpace__searchSpaceType__common__dci_Format2_3__monitoringPeriodicity_n2;
                break;
              case 4:
                NRRRC_CONFIGURATION_REQ (msg_p).Common_dci_Format2_3_monitoringPeriodicity[j] =  SearchSpace__searchSpaceType__common__dci_Format2_3__monitoringPeriodicity_n4;
                break;
              case 5:
                NRRRC_CONFIGURATION_REQ (msg_p).Common_dci_Format2_3_monitoringPeriodicity[j] =  SearchSpace__searchSpaceType__common__dci_Format2_3__monitoringPeriodicity_n5;
                break;
              case 8:
                NRRRC_CONFIGURATION_REQ (msg_p).Common_dci_Format2_3_monitoringPeriodicity[j] =  SearchSpace__searchSpaceType__common__dci_Format2_3__monitoringPeriodicity_n8;
                break;
              case 10:
                NRRRC_CONFIGURATION_REQ (msg_p).Common_dci_Format2_3_monitoringPeriodicity[j] =  SearchSpace__searchSpaceType__common__dci_Format2_3__monitoringPeriodicity_n10;
                break;
              case 16:
                NRRRC_CONFIGURATION_REQ (msg_p).Common_dci_Format2_3_monitoringPeriodicity[j] =  SearchSpace__searchSpaceType__common__dci_Format2_3__monitoringPeriodicity_n16;
                break;
              case 20:
                NRRRC_CONFIGURATION_REQ (msg_p).Common_dci_Format2_3_monitoringPeriodicity[j] =  SearchSpace__searchSpaceType__common__dci_Format2_3__monitoringPeriodicity_n20;
                break;
              default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for Common_dci_Format2_3_monitoringPeriodicity choice: 1,2,4,5,8,10,16,20 !\n",
                             RC.config_file_name, i, Common_dci_Format2_3_monitoringPeriodicity);
                break;
            }

            switch(Common_dci_Format2_3_nrofPDCCH_Candidates){
              case 1:
                NRRRC_CONFIGURATION_REQ (msg_p).Common_dci_Format2_3_nrofPDCCH_Candidates[j] =  SearchSpace__searchSpaceType__common__dci_Format2_3__nrofPDCCH_Candidates_n1;
                break;
              case 2:
                NRRRC_CONFIGURATION_REQ (msg_p).Common_dci_Format2_3_nrofPDCCH_Candidates[j] =  SearchSpace__searchSpaceType__common__dci_Format2_3__nrofPDCCH_Candidates_n2;
                break;
              default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for Common_dci_Format2_3_nrofPDCCH_Candidates choice: 1,2 !\n",
                             RC.config_file_name, i, Common_dci_Format2_3_nrofPDCCH_Candidates);
                break;
            }

            if (strcmp(ue_Specific__dci_Formats , "formats0-0-And-1-0") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).ue_Specific__dci_Formats[j] =  SearchSpace__searchSpaceType__ue_Specific__dci_Formats_formats0_0_And_1_0;
            }else if (strcmp(ue_Specific__dci_Formats , "formats0-1-And-1-1") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).ue_Specific__dci_Formats[j] =  SearchSpace__searchSpaceType__ue_Specific__dci_Formats_formats0_1_And_1_1;
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for ue_Specific__dci_Formats !\n",
                           RC.config_file_name, i, ue_Specific__dci_Formats);
            }

            //////////////////////////////////NR RateMatchPatternLTE-CRS///////////////////////////

            switch(RateMatchPatternLTE_CRS_carrierBandwidthDL){
              case 6:
                NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_carrierBandwidthDL[j] =  RateMatchPatternLTE_CRS__carrierBandwidthDL_n6;
                break;
              case 15:
                NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_carrierBandwidthDL[j] =  RateMatchPatternLTE_CRS__carrierBandwidthDL_n15;
                break;
              case 25:
                NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_carrierBandwidthDL[j] =  RateMatchPatternLTE_CRS__carrierBandwidthDL_n25;
                break;
              case 50:
                NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_carrierBandwidthDL[j] =  RateMatchPatternLTE_CRS__carrierBandwidthDL_n50;
                break;
              case 75:
                NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_carrierBandwidthDL[j] =  RateMatchPatternLTE_CRS__carrierBandwidthDL_n75;
                break;
              case 100:
                NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_carrierBandwidthDL[j] =  RateMatchPatternLTE_CRS__carrierBandwidthDL_n100;
                break;
              default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for RateMatchPatternLTE_CRS_carrierBandwidthDL choice: 6,15,25,50,75,100 !\n",
                             RC.config_file_name, i, RateMatchPatternLTE_CRS_carrierBandwidthDL);
                break;
            }

            NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_carrierBandwidthDL[j] = RateMatchPatternLTE_CRS_carrierBandwidthDL;
            if ((RateMatchPatternLTE_CRS_carrierBandwidthDL <0) || (RateMatchPatternLTE_CRS_carrierBandwidthDL>16383)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for RateMatchPatternLTE_CRS_carrierBandwidthDL choice: 0..16383 !\n",
                           RC.config_file_name, i, RateMatchPatternLTE_CRS_carrierBandwidthDL);
            }

            switch(RateMatchPatternLTE_CRS_nrofCRS_Ports){
              case 1:
                NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_nrofCRS_Ports[j] =  RateMatchPatternLTE_CRS__nrofCRS_Ports_n1;
                break;
              case 2:
                NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_nrofCRS_Ports[j] =  RateMatchPatternLTE_CRS__nrofCRS_Ports_n2;
                break;
              case 4:
                NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_nrofCRS_Ports[j] =  RateMatchPatternLTE_CRS__nrofCRS_Ports_n4;
                break;
              default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for RateMatchPatternLTE_CRS_nrofCRS_Ports choice: 1,2,4 !\n",
                             RC.config_file_name, i, RateMatchPatternLTE_CRS_nrofCRS_Ports);
                break;
            }

            switch(RateMatchPatternLTE_CRS_v_Shift){
              case 0:
                NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_v_Shift[j] =  RateMatchPatternLTE_CRS__v_Shift_n0;
                break;
              case 1:
                NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_v_Shift[j] =  RateMatchPatternLTE_CRS__v_Shift_n1;
                break;
              case 2:
                NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_v_Shift[j] =  RateMatchPatternLTE_CRS__v_Shift_n2;
                break;
              case 3:
                NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_v_Shift[j] =  RateMatchPatternLTE_CRS__v_Shift_n3;
                break;
              case 4:
                NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_v_Shift[j] =  RateMatchPatternLTE_CRS__v_Shift_n4;
                break;
              case 5:
                NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_v_Shift[j] =  RateMatchPatternLTE_CRS__v_Shift_n5;
                break;
              default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for RateMatchPatternLTE_CRS_v_Shift choice: 0,1,2,3,4,5 !\n",
                             RC.config_file_name, i, RateMatchPatternLTE_CRS_v_Shift);
                break;
            }

            switch(RateMatchPatternLTE_CRS_radioframeAllocationPeriod){
              case 1:
                NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_radioframeAllocationPeriod[j] =  EUTRA_MBSFN_SubframeConfig__radioframeAllocationPeriod_n1;
                break;
              case 2:
                NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_radioframeAllocationPeriod[j] =  EUTRA_MBSFN_SubframeConfig__radioframeAllocationPeriod_n2;
                break;
              case 4:
                NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_radioframeAllocationPeriod[j] =  EUTRA_MBSFN_SubframeConfig__radioframeAllocationPeriod_n4;
                break;
              case 8:
                NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_radioframeAllocationPeriod[j] =  EUTRA_MBSFN_SubframeConfig__radioframeAllocationPeriod_n8;
                break;
              case 16:
                NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_radioframeAllocationPeriod[j] =  EUTRA_MBSFN_SubframeConfig__radioframeAllocationPeriod_n16;
                break;
              case 32:
                NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_radioframeAllocationPeriod[j] =  EUTRA_MBSFN_SubframeConfig__radioframeAllocationPeriod_n32;
                break;
              default:
                AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for RateMatchPatternLTE_CRS_radioframeAllocationPeriod choice: 1,2,4,8,16,32 !\n",
                             RC.config_file_name, i, RateMatchPatternLTE_CRS_radioframeAllocationPeriod);
                break;
            }

            NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_radioframeAllocationOffset[j] = RateMatchPatternLTE_CRS_radioframeAllocationOffset;
            if ((RateMatchPatternLTE_CRS_radioframeAllocationOffset <0) || (RateMatchPatternLTE_CRS_radioframeAllocationOffset>7)){
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for RateMatchPatternLTE_CRS_radioframeAllocationOffset choice: 0..7 !\n",
                           RC.config_file_name, i, RateMatchPatternLTE_CRS_radioframeAllocationOffset);
            }

            if (strcmp(RateMatchPatternLTE_CRS_subframeAllocation_choice , "oneFrame") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_subframeAllocation_choice[j] =  EUTRA_MBSFN_SubframeConfig__subframeAllocation_PR_oneFrame;
            }else if (strcmp(RateMatchPatternLTE_CRS_subframeAllocation_choice , "fourFrames") == 0){
              NRRRC_CONFIGURATION_REQ (msg_p).RateMatchPatternLTE_CRS_subframeAllocation_choice[j] =  EUTRA_MBSFN_SubframeConfig__subframeAllocation_PR_fourFrames;
            }else {
              AssertFatal (0,"Failed to parse gNB configuration file %s, gnb %d unknown value \"%d\" for RateMatchPatternLTE_CRS_subframeAllocation_choice !\n",
                           RC.config_file_name, i, RateMatchPatternLTE_CRS_subframeAllocation_choice);
            }


          }//End for (j = 0; j < CCsParamList.numelt ;j++)

        }//End if ( CCsParamList.numelt> 0) 

      }//End for (k=0; k <num_gnbs ; k++)

    }//End for (k=0; k <num_gnbs ; k++)


  }//End if (num_gnbs>0)


}//End RCconfig_NRRRC function

void NRRCConfig(void) {

  // paramlist_def_t MACRLCParamList = {CONFIG_STRING_MACRLC_LIST,NULL,0};
  // paramlist_def_t L1ParamList = {CONFIG_STRING_L1_LIST,NULL,0};
  // paramlist_def_t RUParamList = {CONFIG_STRING_RU_LIST,NULL,0};
  paramdef_t GNBSParams[] = GNBSPARAMS_DESC;
  paramlist_def_t CCsParamList = {GNB_CONFIG_STRING_COMPONENT_CARRIERS,NULL,0};
  
  char aprefix[MAX_OPTNAME_SIZE*2 + 8];  
  


/* get global parameters, defined outside any section in the config file */
 
  printf("Getting GNBSParams\n");
 
  config_get( GNBSParams,sizeof(GNBSParams)/sizeof(paramdef_t),NULL); 
  RC.nb_nr_inst = GNBSParams[GNB_ACTIVE_GNBS_IDX].numelt;
 
  if (RC.nb_nr_inst > 0) {
    RC.nb_CC = (int *)malloc((1+RC.nb_nr_inst)*sizeof(int));
    for (int i=0;i<RC.nb_nr_inst;i++) {
      sprintf(aprefix,"%s.[%i]",GNB_CONFIG_STRING_GNB_LIST,i);
      config_getlist( &CCsParamList,NULL,0, aprefix);
      RC.nb_CC[i]   = CCsParamList.numelt;
    }
  }

/*    
	// Get num MACRLC instances
    config_getlist( &MACRLCParamList,NULL,0, NULL);
    RC.nb_macrlc_inst  = MACRLCParamList.numelt;
    // Get num L1 instances
    config_getlist( &L1ParamList,NULL,0, NULL);
    RC.nb_L1_inst = L1ParamList.numelt;

    // Get num RU instances
    config_getlist( &RUParamList,NULL,0, NULL);  
    RC.nb_RU     = RUParamList.numelt; 
 */
 

}
