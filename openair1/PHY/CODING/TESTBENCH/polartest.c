#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "PHY/CODING/nrPolar_tools/nr_polar_defs.h"
#include "PHY/CODING/nrPolar_tools/nr_polar_pbch_defs.h"
#include "SIMULATION/TOOLS/sim.h"

int main(int argc, char *argv[]) {

	//Initiate timing. (Results depend on CPU Frequency. Therefore, might change due to performance variances during simulation.)
	time_stats_t timeEncoder,timeDecoder;
	opp_enabled=1;
	cpu_freq_GHz = get_cpu_freq_GHz();
	reset_meas(&timeEncoder);
	reset_meas(&timeDecoder);

	randominit(0);
	//Default simulation values (Aim for iterations = 1000000.)
	int itr, iterations = 1000, arguments, polarMessageType = 1; //0=DCI, 1=PBCH, 2=UCI
	double SNRstart = -20.0, SNRstop = 0.0, SNRinc= 0.5; //dB

	double SNR, SNR_lin;
	int16_t nBitError = 0; // -1 = Decoding failed (All list entries have failed the CRC checks).
	int8_t decoderState=0, blockErrorState=0; //0 = Success, -1 = Decoding failed, 1 = Block Error.
	uint16_t testLength, coderLength, blockErrorCumulative=0, bitErrorCumulative=0;
	double timeEncoderCumulative = 0, timeDecoderCumulative = 0;

	uint8_t decoderListSize = 8, pathMetricAppr = 0; //0 --> eq. (8a) and (11b), 1 --> eq. (9) and (12)

	while ((arguments = getopt (argc, argv, "s:d:f:m:i:l:a:")) != -1)
	switch (arguments)
	{
		case 's':
			SNRstart = atof(optarg);
			printf("SNRstart = %f\n", SNRstart);
			break;

		case 'd':
			SNRinc = atof(optarg);
			break;

		case 'f':
			SNRstop = atof(optarg);
			break;

		case 'm':
			polarMessageType = atoi(optarg);
			break;

		case 'i':
			iterations = atoi(optarg);
			break;

		case 'l':
			decoderListSize = (uint8_t) atoi(optarg);
			break;

		case 'a':
			pathMetricAppr = (uint8_t) atoi(optarg);
			break;

		default:
			perror("[polartest.c] Problem at argument parsing with getopt");
			abort ();
	}

	if (polarMessageType == 0) { //DCI
		//testLength = ;
		//coderLength = ;
	} else if (polarMessageType == 1) { //PBCH
		testLength = NR_POLAR_PBCH_PAYLOAD_BITS;
		coderLength = NR_POLAR_PBCH_E;
	} else if (polarMessageType == 2) { //UCI
		//testLength = ;
		//coderLength = ;
	}

	//Logging
	time_t currentTime;
	time (&currentTime);
	char *folderName, fileName[512], currentTimeInfo[25];

	folderName=getenv("HOME");
	strcat(folderName,"/Desktop/polartestResults");
	sprintf(fileName,"%s/_ListSize_%d_pmAppr_%d_Payload_%d_Itr_%d",folderName,decoderListSize,pathMetricAppr,testLength,iterations);
	strftime(currentTimeInfo, 25, "_%Y-%m-%d-%H-%M-%S.csv", localtime(&currentTime));
	strcat(fileName,currentTimeInfo);

	//Create "~/Desktop/polartestResults" folder if it doesn't already exist.
	struct stat folder = {0};
	if (stat(folderName, &folder) == -1) mkdir(folderName, S_IRWXU | S_IRWXG | S_IRWXO);

	FILE* logFile;
    logFile = fopen(fileName, "w");
    if (logFile==NULL) {
        fprintf(stderr,"[polartest.c] Problem creating file %s with fopen\n",fileName);
        exit(-1);
      }
    fprintf(logFile,",SNR,nBitError,blockErrorState,t_encoder[us],t_decoder[us]\n");

	uint8_t *testInput = malloc(sizeof(uint8_t) * testLength); //generate randomly
	uint8_t *encoderOutput = malloc(sizeof(uint8_t) * coderLength);
	double *modulatedInput = malloc (sizeof(double) * coderLength); //channel input

	double *channelOutput  = malloc (sizeof(double) * coderLength); //add noise
	uint8_t *estimatedOutput = malloc(sizeof(uint8_t) * testLength); //decoder output

	t_nrPolar_params nrPolar_params;
	nr_polar_init(&nrPolar_params, polarMessageType);

	// We assume no a priori knowledge available about the payload.
	double aPrioriArray[nrPolar_params.payloadBits];
	for (int i=0; i<nrPolar_params.payloadBits; i++) aPrioriArray[i] = NAN;

	for (SNR = SNRstart; SNR <= SNRstop; SNR += SNRinc) {
		SNR_lin = pow(10, SNR/10);
		for (itr = 1; itr <= iterations; itr++) {

		for(int i=0; i<testLength; i++) testInput[i]=(uint8_t) (rand() % 2);

		start_meas(&timeEncoder);
		polar_encoder(testInput, encoderOutput, &nrPolar_params);
		stop_meas(&timeEncoder);

		//BPSK modulation
		for(int i=0; i<coderLength; i++) {
			if (encoderOutput[i] == 0)
				modulatedInput[i]=1/sqrt(2);
			else
				modulatedInput[i]=(-1)/sqrt(2);

			channelOutput[i] = modulatedInput[i] + (gaussdouble(0.0,1.0) * (1/sqrt(2*SNR_lin)));
		}


		start_meas(&timeDecoder);
		decoderState = polar_decoder(channelOutput, estimatedOutput, &nrPolar_params, decoderListSize, aPrioriArray, pathMetricAppr);
		stop_meas(&timeDecoder);

		//calculate errors
		if (decoderState==-1) {
			blockErrorState=-1;
			nBitError=-1;
		} else {
			for(int i=0; i<testLength; i++){
				if (estimatedOutput[i]!=testInput[i]) nBitError++;
			}
			if (nBitError>0) blockErrorState=1;
		}

		//Iteration times are in microseconds.
		timeEncoderCumulative+=(timeEncoder.diff_now/(cpu_freq_GHz*1000.0));
		timeDecoderCumulative+=(timeDecoder.diff_now/(cpu_freq_GHz*1000.0));
		fprintf(logFile,",%f,%d,%d,%f,%f\n", SNR, nBitError, blockErrorState,
				(timeEncoder.diff_now/(cpu_freq_GHz*1000.0)), (timeDecoder.diff_now/(cpu_freq_GHz*1000.0)));

		if (nBitError<0) {
			blockErrorCumulative++;
			bitErrorCumulative+=testLength;
		} else {
			blockErrorCumulative+=blockErrorState;
			bitErrorCumulative+=nBitError;
		}

		decoderState=0;
		nBitError=0;
		blockErrorState=0;

		}
		//Calculate error statistics for the SNR.
		printf("[ListSize=%d, Appr=%d] SNR=%+8.3f, BLER=%9.6f, BER=%12.9f, t_Encoder=%9.3fus, t_Decoder=%9.3fus\n",
				decoderListSize, pathMetricAppr, SNR, ((double)blockErrorCumulative/iterations),
				((double)bitErrorCumulative / (iterations*testLength)),
				(timeEncoderCumulative/iterations),timeDecoderCumulative/iterations);

		blockErrorCumulative = 0; bitErrorCumulative = 0;
		timeEncoderCumulative = 0; timeDecoderCumulative = 0;
	}

	print_meas(&timeEncoder,"polar_encoder",NULL,NULL);
	print_meas(&timeDecoder,"polar_decoder",NULL,NULL);

	fclose(logFile);
	free(testInput);
	free(encoderOutput);
	free(modulatedInput);
	free(channelOutput);
	free(estimatedOutput);

	return (0);
}
