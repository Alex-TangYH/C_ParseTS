#include <stdio.h>
#include <string.h>
#include "Parse_Descriptor.h"
#include "Print_Descriptor.h"

#define VIDEO_STREAM_DESCRIPTOR_TAG 0x02
#define AUDIO_STREAM_DESCRIPTOR_TAG 0x03
#define DATA_STREAM_ALIGNMENT_DESCRIPTOR_TAG 0x06
#define CA_DESCRIPTOR_TAG 0x09
#define ISO_639_LANGUAGE_DESCRIPTOR_TAG 0x10
#define SYSTEM_CLOCK_DESCRIPTOR_TAG 0XB
#define MAXIMUM_BITRATE_DESCRIPTOR_TAG 0x0e

#define NETWORK_NAME_DESCRIPTOR_TAG 0x40
#define SERVICE_LIST_DESCRIPTOR_TAG 0x41
#define SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR_TAG 0x43
#define CABLE_DELIVERY_SYSTEM_DESCRIPTOR_TAG 0x44
#define BOUQUET_NAME_DESCRIPTOR_TAG 0x47
#define SERVICE_DESCRIPTOR_TAG 0x48
#define LINKAGE_DESCRIPTOR_TAG 0x4a
#define SHORT_EVENT_DESCRIPTOR_TAG 0x4d
#define EXTENDED_EVENT_DESCRIPTOR_TAG 0x4e
#define STREAM_IDENTIFIER_DESCRIPTOR_TAG 0x52
#define TELETEXT_DESCRIPTOR_TAG 0x56
#define SUBTITLING_DESCRIPTOR_TAG 0x59
#define TERRESTRIAL_DELIVERY_SYSTEM_DESCRIPTOR_TAG 0x5a
#define FREQUENCY_LIST_DESCRIPTOR_TAG 0x62

/******************************************
 *
 * 解析Network Name Descriptor描述子信息
 *
 ******************************************/
int GetNetworkNameDescriptor(NETWORK_NAME_DESCRIPTOR_T *pstNetworkNameDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition)
{
	memset(pstNetworkNameDescriptor, 0, sizeof(NETWORK_NAME_DESCRIPTOR_T));
	pstNetworkNameDescriptor->uiDescriptor_tag = pucDescriptorBuffer[iDescriptorPosition];
	pstNetworkNameDescriptor->uiDescriptor_length = pucDescriptorBuffer[1 + iDescriptorPosition];
	memcpy(pstNetworkNameDescriptor->aucInfo, pucDescriptorBuffer + 2 + iDescriptorPosition, pstNetworkNameDescriptor->uiDescriptor_length);
	return iDescriptorPosition;
}

/******************************************
 *
 * 解析Service List Descriptor描述子信息
 *
 ******************************************/
int GetServiceListDescriptor(SERVICE_LIST_DESCRIPTOR_T *pstServiceListDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition)
{
	int iDescriptorInfoPosition = 0;
	int iDescriptorInfoCount = 0;
	
	memset(pstServiceListDescriptor, 0, sizeof(SERVICE_LIST_DESCRIPTOR_T));
	pstServiceListDescriptor->uiDescriptor_tag = pucDescriptorBuffer[iDescriptorPosition];
	pstServiceListDescriptor->uiDescriptor_length = pucDescriptorBuffer[1 + iDescriptorPosition];
	for (iDescriptorInfoPosition = 0; iDescriptorInfoPosition < pstServiceListDescriptor->uiDescriptor_length; iDescriptorInfoPosition += 3)
	{
		if (1 == pstServiceListDescriptor->uiDescriptor_length - iDescriptorInfoPosition)
		{
			pstServiceListDescriptor->astService_info[iDescriptorInfoCount].uiServiec_id = pucDescriptorBuffer[iDescriptorPosition + 2 + iDescriptorInfoPosition];
			break;
		}
		else
		{
			pstServiceListDescriptor->astService_info[iDescriptorInfoCount].uiServiec_id = (pucDescriptorBuffer[iDescriptorPosition + 2 + iDescriptorInfoPosition] << 8) | pucDescriptorBuffer[iDescriptorPosition + 2 + iDescriptorInfoPosition + 1];
		}

		if (pstServiceListDescriptor->uiDescriptor_length - iDescriptorInfoPosition > 2)
		{
			pstServiceListDescriptor->astService_info[iDescriptorInfoCount].uiService_type = pucDescriptorBuffer[iDescriptorPosition + 2 + iDescriptorInfoPosition + 2];
		}
		else
		{
		}
		iDescriptorInfoCount++;
	}
	return iDescriptorPosition;
}

/**********************************************************
 *
 * 解析Cable Delivery System Descriptor描述子信息
 *
 **********************************************************/
int GetCableDeliverySystemDescriptor(CABLE_DELIVERY_SYSTEM_DESCRIPTOR_T *pstCableDeliverySystemDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition)
{
	memset(pstCableDeliverySystemDescriptor, 0, sizeof(SERVICE_LIST_DESCRIPTOR_T));
	pstCableDeliverySystemDescriptor->uiDescriptor_tag = pucDescriptorBuffer[iDescriptorPosition];
	pstCableDeliverySystemDescriptor->uiDescriptor_length = pucDescriptorBuffer[1 + iDescriptorPosition];
	if (pstCableDeliverySystemDescriptor->uiDescriptor_length > 0)
	{
		pstCableDeliverySystemDescriptor->uiFrequency = (pucDescriptorBuffer[2 + iDescriptorPosition] << 24) | (pucDescriptorBuffer[3 + iDescriptorPosition] << 16) | (pucDescriptorBuffer[4 + iDescriptorPosition] << 8)
				| (pucDescriptorBuffer[5 + iDescriptorPosition]);
		pstCableDeliverySystemDescriptor->uiReserved_future_use = (pucDescriptorBuffer[6 + iDescriptorPosition] << 4) | (pucDescriptorBuffer[7 + iDescriptorPosition] >> 4);
		pstCableDeliverySystemDescriptor->uiFec_outer = pucDescriptorBuffer[8 + iDescriptorPosition] & 0x0f;
		pstCableDeliverySystemDescriptor->uiModulation = pucDescriptorBuffer[9 + iDescriptorPosition];
		pstCableDeliverySystemDescriptor->uiSymbol_rate = (pucDescriptorBuffer[10 + iDescriptorPosition] << 20) | (pucDescriptorBuffer[11 + iDescriptorPosition] << 12) | (pucDescriptorBuffer[12 + iDescriptorPosition] << 4)
				| (pucDescriptorBuffer[13 + iDescriptorPosition] >> 4);
		pstCableDeliverySystemDescriptor->uiFEC_inner = pucDescriptorBuffer[13 + iDescriptorPosition] & 0x0f;
	}
	else
	{
	}
	return iDescriptorPosition;
}

/**********************************************************
 *
 * 解析Satellite Delivery System Descriptor描述子信息
 *
 **********************************************************/
int GetSatelliteDeliverySystemDescriptor(SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR_T *pstSatelliteDeliverySystemDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition)
{
	memset(pstSatelliteDeliverySystemDescriptor, 0, sizeof(SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR_T));
	pstSatelliteDeliverySystemDescriptor->uiDescriptor_tag = pucDescriptorBuffer[iDescriptorPosition];
	pstSatelliteDeliverySystemDescriptor->uiDescriptor_length = pucDescriptorBuffer[1 + iDescriptorPosition];
	if (pstSatelliteDeliverySystemDescriptor->uiDescriptor_length > 0)
	{
		pstSatelliteDeliverySystemDescriptor->uiFrequency = (pucDescriptorBuffer[2 + iDescriptorPosition] << 24) | (pucDescriptorBuffer[3 + iDescriptorPosition] << 16) | (pucDescriptorBuffer[4 + iDescriptorPosition] << 8)
				| (pucDescriptorBuffer[5 + iDescriptorPosition]);
		pstSatelliteDeliverySystemDescriptor->uiOrbital_position = (pucDescriptorBuffer[6 + iDescriptorPosition] << 8) | (pucDescriptorBuffer[7 + iDescriptorPosition]);
		pstSatelliteDeliverySystemDescriptor->uiWest_east_flag = pucDescriptorBuffer[8 + iDescriptorPosition] >> 7;
		pstSatelliteDeliverySystemDescriptor->uiPolarization = (pucDescriptorBuffer[8 + iDescriptorPosition] >> 5) & 0x03;
		pstSatelliteDeliverySystemDescriptor->uiModulation_system = (pucDescriptorBuffer[8 + iDescriptorPosition] >> 2) & 0x01;
		if (1 == pstSatelliteDeliverySystemDescriptor->uiModulation_system)
		{
			pstSatelliteDeliverySystemDescriptor->uiRoll_off = (pucDescriptorBuffer[8 + iDescriptorPosition] >> 3) & 0x03;
		}
		else
		{
			pstSatelliteDeliverySystemDescriptor->uiRoll_off = 0x00;
		}
		pstSatelliteDeliverySystemDescriptor->uiModulation_type = pucDescriptorBuffer[8 + iDescriptorPosition] & 0x03;
		pstSatelliteDeliverySystemDescriptor->uiSymbol_rate = (pucDescriptorBuffer[9 + iDescriptorPosition] << 20) | (pucDescriptorBuffer[10 + iDescriptorPosition] << 12) | (pucDescriptorBuffer[11 + iDescriptorPosition] << 4)
				| (pucDescriptorBuffer[12 + iDescriptorPosition] >> 4);
		pstSatelliteDeliverySystemDescriptor->uiFEC_inner = pucDescriptorBuffer[12 + iDescriptorPosition] & 0x0f;
	}
	else
	{
	}
	return iDescriptorPosition;
}

/******************************************
 *
 * 解析Network Name Descriptor描述子信息
 *
 ******************************************/
int GetBouquetNameDescriptor(BOUQUET_NAME_DESCRIPTOR_T *pstBouquetNameDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition)
{
	memset(pstBouquetNameDescriptor, 0, sizeof(BOUQUET_NAME_DESCRIPTOR_T));
	pstBouquetNameDescriptor->uiDescriptor_tag = pucDescriptorBuffer[iDescriptorPosition];
	pstBouquetNameDescriptor->uiDescriptor_length = pucDescriptorBuffer[1 + iDescriptorPosition];
	memcpy(pstBouquetNameDescriptor->aucInfo, pucDescriptorBuffer + 2 + iDescriptorPosition, pstBouquetNameDescriptor->uiDescriptor_length);
	return iDescriptorPosition;
}

/******************************************
 *
 * 解析Service   Descriptor描述子信息
 *
 ******************************************/
int GetServiceDescriptor(SERVICE_DESCRIPTOR_T *pstServiceDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition)
{
	memset(pstServiceDescriptor, 0, sizeof(SERVICE_DESCRIPTOR_T));
	pstServiceDescriptor->uiDescriptor_tag = pucDescriptorBuffer[iDescriptorPosition];
	pstServiceDescriptor->uiDescriptor_length = pucDescriptorBuffer[1 + iDescriptorPosition];
	pstServiceDescriptor->uiService_type = pucDescriptorBuffer[2 + iDescriptorPosition];
	pstServiceDescriptor->uiService_provider_name_length = pucDescriptorBuffer[3 + iDescriptorPosition];
	if (pstServiceDescriptor->uiService_provider_name_length > 0)
	{
		memcpy(pstServiceDescriptor->aucService_provider_name, pucDescriptorBuffer + iDescriptorPosition + 4, pstServiceDescriptor->uiService_provider_name_length);
	}
	else
	{
	}
	pstServiceDescriptor->uiService_name_length = pucDescriptorBuffer[4 + iDescriptorPosition + pstServiceDescriptor->uiService_provider_name_length];
	if (pstServiceDescriptor->uiService_name_length > 0)
	{
		memcpy(pstServiceDescriptor->aucService_name, pucDescriptorBuffer + iDescriptorPosition + 5 + pstServiceDescriptor->uiService_provider_name_length, pstServiceDescriptor->uiService_name_length);
	}
	else
	{
	}
	return iDescriptorPosition;
}

/******************************************
 *
 * 解析Link   Descriptor描述子信息
 *
 ******************************************/
int GetLinkageDescriptor(LINKAGE_DESCRIPTOR_T *pstLinkageDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition)
{
	int iLengthBeforePrivateData = 0;
	memset(pstLinkageDescriptor, 0, sizeof(LINKAGE_DESCRIPTOR_T));
	pstLinkageDescriptor->uiDescriptor_tag = pucDescriptorBuffer[iDescriptorPosition + 0];
	pstLinkageDescriptor->uiDescriptor_length = pucDescriptorBuffer[iDescriptorPosition + 1];
	pstLinkageDescriptor->uiTransport_stream_id = (pucDescriptorBuffer[iDescriptorPosition + 2] << 8) | pucDescriptorBuffer[iDescriptorPosition + 3];
	pstLinkageDescriptor->uiOriginal_network_id = (pucDescriptorBuffer[iDescriptorPosition + 4] << 8) | pucDescriptorBuffer[iDescriptorPosition + 5];
	pstLinkageDescriptor->uiService_id = (pucDescriptorBuffer[iDescriptorPosition + 6] << 8) | pucDescriptorBuffer[iDescriptorPosition + 7];
	pstLinkageDescriptor->uiLinage_type = pucDescriptorBuffer[iDescriptorPosition + 8];
	iLengthBeforePrivateData = 7;
	if (0x08 == pstLinkageDescriptor->uiLinage_type)
	{
		pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiHand_over_type = pucDescriptorBuffer[iDescriptorPosition + 9] >> 4;
		pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiReserved_future_use = (pucDescriptorBuffer[iDescriptorPosition + 9] >> 1) & 0x3;
		pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiOrigin_type = (pucDescriptorBuffer[iDescriptorPosition + 9] << 7) >> 7;
		iLengthBeforePrivateData += 1;
		if ((1 == pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiHand_over_type) || (2 == pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiHand_over_type)
				|| (3 == pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiHand_over_type))
		{
			pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiNetwork_id = (pucDescriptorBuffer[iDescriptorPosition + 10] << 8) | pucDescriptorBuffer[iDescriptorPosition + 11];
			iLengthBeforePrivateData += 2;
		}
		else
		{
		}
		if (0 == pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiOrigin_type)
		{
			pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiInitial_service_id = (pucDescriptorBuffer[iDescriptorPosition + 10] << 8) | pucDescriptorBuffer[iDescriptorPosition + 11];
			iLengthBeforePrivateData += 2;
		}
		else
		{
		}
	}
	else if (0x0D == pstLinkageDescriptor->uiLinage_type)
	{
		pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_0d.uiTarget_event_id = (pucDescriptorBuffer[iDescriptorPosition + 9] << 8) | pucDescriptorBuffer[iDescriptorPosition + 10];
		pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_0d.uiTarget_listed = pucDescriptorBuffer[iDescriptorPosition + 11] >> 7;
		pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_0d.uiEvent_simulcast = (pucDescriptorBuffer[iDescriptorPosition + 11] >> 6) & 0x01;
		pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_0d.uiReserved = pucDescriptorBuffer[iDescriptorPosition + 11] & 0x3f;
		iLengthBeforePrivateData += 3;
	}
	else
	{
	}
	memcpy(pstLinkageDescriptor->aucPrivate_data, pucDescriptorBuffer + iLengthBeforePrivateData + iDescriptorPosition, pstLinkageDescriptor->uiDescriptor_length - iLengthBeforePrivateData);
	return iDescriptorPosition;
}

/******************************************
 *
 * 解析Stream Indentifier Descriptor描述子信息
 *
 ******************************************/
int GetStreamIndentifierDescriptor(STREAM_IDENTIFIER_DESCRIPTOR_T *pstStreamIndentifierDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition)
{
	memset(pstStreamIndentifierDescriptor, 0, sizeof(STREAM_IDENTIFIER_DESCRIPTOR_T));
	pstStreamIndentifierDescriptor->uiDescriptor_tag = pucDescriptorBuffer[iDescriptorPosition];
	pstStreamIndentifierDescriptor->uiDescriptor_length = pucDescriptorBuffer[1 + iDescriptorPosition];
	pstStreamIndentifierDescriptor->uiComponent_tag = pucDescriptorBuffer[2 + iDescriptorPosition];
	return iDescriptorPosition;
}

/******************************************
 *
 * 解析Teletext Descriptor描述子信息
 *
 ******************************************/
int GetTeletextDescriptor(TELETEXT_DESCRIPTOR_T *pstTeletextDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition)
{
	int iLoopCount = 0;
	int iDescriptorInfoPostion = 0;
	memset(pstTeletextDescriptor, 0, sizeof(TELETEXT_DESCRIPTOR_T));
	pstTeletextDescriptor->uiDescriptor_tag = pucDescriptorBuffer[iDescriptorPosition];
	pstTeletextDescriptor->uiDescriptor_length = pucDescriptorBuffer[1 + iDescriptorPosition];
	for (iDescriptorInfoPostion = 0; iDescriptorInfoPostion < pstTeletextDescriptor->uiDescriptor_length; iDescriptorInfoPostion += 5)
	{
		pstTeletextDescriptor->astTeletext_Info[iLoopCount].stISO_639_Language_code.aucPrivate_data_byte[0] = pucDescriptorBuffer[2 + iDescriptorInfoPostion];
		pstTeletextDescriptor->astTeletext_Info[iLoopCount].stISO_639_Language_code.aucPrivate_data_byte[1] = pucDescriptorBuffer[3 + iDescriptorInfoPostion];
		pstTeletextDescriptor->astTeletext_Info[iLoopCount].stISO_639_Language_code.aucPrivate_data_byte[2] = pucDescriptorBuffer[4 + iDescriptorInfoPostion];
		pstTeletextDescriptor->astTeletext_Info[iLoopCount].uiTeletext_type = pucDescriptorBuffer[5 + iDescriptorInfoPostion] >> 3;
		pstTeletextDescriptor->astTeletext_Info[iLoopCount].uiTeletext_magazine_number = pucDescriptorBuffer[5 + iDescriptorInfoPostion] & 0x07;
		pstTeletextDescriptor->astTeletext_Info[iLoopCount].uiTeletext_page_number = pucDescriptorBuffer[6 + iDescriptorInfoPostion];
		iLoopCount++;
	}
	return iDescriptorPosition;
}

/******************************************
 *
 * 解析Local Time Offset Descriptor描述子信息
 *
 ******************************************/
int GetLocalTimeOffsetDescriptor(LOCAL_TIME_OFFSET_DESCRIPTOR_T *pstLocalTimeOffsetDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition)
{
	int iLoopCount = 0;
	int iOneInfoLength = 3 + 1 + 2 + 5 + 2;
	memset(pstLocalTimeOffsetDescriptor, 0, sizeof(LOCAL_TIME_OFFSET_DESCRIPTOR_T));
	pstLocalTimeOffsetDescriptor->uiDescriptor_tag = pucDescriptorBuffer[iDescriptorPosition];
	pstLocalTimeOffsetDescriptor->uiDescriptor_length = pucDescriptorBuffer[1 + iDescriptorPosition];
	for (iLoopCount = 0; iLoopCount * iOneInfoLength < pstLocalTimeOffsetDescriptor->uiDescriptor_length; iLoopCount++)
	{
		pstLocalTimeOffsetDescriptor->astLocalTimeOffset_Info[iLoopCount].uiCountry_code[0] = pucDescriptorBuffer[iDescriptorPosition + 2 + iLoopCount * iOneInfoLength];
		pstLocalTimeOffsetDescriptor->astLocalTimeOffset_Info[iLoopCount].uiCountry_code[1] = pucDescriptorBuffer[iDescriptorPosition + 3 + iLoopCount * iOneInfoLength];
		pstLocalTimeOffsetDescriptor->astLocalTimeOffset_Info[iLoopCount].uiCountry_code[2] = pucDescriptorBuffer[iDescriptorPosition + 4 + iLoopCount * iOneInfoLength];
		pstLocalTimeOffsetDescriptor->astLocalTimeOffset_Info[iLoopCount].uiCountry_region_id = pucDescriptorBuffer[iDescriptorPosition + 5 + iLoopCount * iOneInfoLength] >> 2;
		pstLocalTimeOffsetDescriptor->astLocalTimeOffset_Info[iLoopCount].uiReserved = (pucDescriptorBuffer[iDescriptorPosition + 5 + iLoopCount * iOneInfoLength] >> 1) & 0x01;
		pstLocalTimeOffsetDescriptor->astLocalTimeOffset_Info[iLoopCount].uiLocal_time_offset_polarity = (pucDescriptorBuffer[iDescriptorPosition + 5 + iLoopCount * iOneInfoLength] << 7) >> 7;
		pstLocalTimeOffsetDescriptor->astLocalTimeOffset_Info[iLoopCount].uiLocal_time_offset = (pucDescriptorBuffer[iDescriptorPosition + 6 + iLoopCount * iOneInfoLength] << 8)
				| (pucDescriptorBuffer[iDescriptorPosition + 7 + iLoopCount * iOneInfoLength]);
		pstLocalTimeOffsetDescriptor->astLocalTimeOffset_Info[iLoopCount].uiTime_of_change[0] = pucDescriptorBuffer[iDescriptorPosition + 8 + iLoopCount * iOneInfoLength];
		pstLocalTimeOffsetDescriptor->astLocalTimeOffset_Info[iLoopCount].uiTime_of_change[1] = pucDescriptorBuffer[iDescriptorPosition + 9 + iLoopCount * iOneInfoLength];
		pstLocalTimeOffsetDescriptor->astLocalTimeOffset_Info[iLoopCount].uiTime_of_change[2] = pucDescriptorBuffer[iDescriptorPosition + 10 + iLoopCount * iOneInfoLength];
		pstLocalTimeOffsetDescriptor->astLocalTimeOffset_Info[iLoopCount].uiTime_of_change[3] = pucDescriptorBuffer[iDescriptorPosition + 11 + iLoopCount * iOneInfoLength];
		pstLocalTimeOffsetDescriptor->astLocalTimeOffset_Info[iLoopCount].uiTime_of_change[4] = pucDescriptorBuffer[iDescriptorPosition + 12 + iLoopCount * iOneInfoLength];
		pstLocalTimeOffsetDescriptor->astLocalTimeOffset_Info[iLoopCount].uiNext_time_offset = (pucDescriptorBuffer[iDescriptorPosition + 13 + iLoopCount * iOneInfoLength] << 8)
				| (pucDescriptorBuffer[iDescriptorPosition + 14 + iLoopCount * iOneInfoLength]);
	}
	return iDescriptorPosition;
}
/******************************************
 *
 * 解析Subtitling Descriptor描述子信息
 *
 ******************************************/
int GetSubtitlingDescriptor(SUBTITLING_DESCRIPTOR_T *pstSubtitlingDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition)
{
	int iLoopCount = 0;
	int iOneInfoLength = 8;
	memset(pstSubtitlingDescriptor, 0, sizeof(SUBTITLING_DESCRIPTOR_T));
	pstSubtitlingDescriptor->uiDescriptor_tag = pucDescriptorBuffer[iDescriptorPosition];
	pstSubtitlingDescriptor->uiDescriptor_length = pucDescriptorBuffer[1 + iDescriptorPosition];
	for (iLoopCount = 0; iLoopCount * iOneInfoLength < pstSubtitlingDescriptor->uiDescriptor_length; iLoopCount++)
	{
		pstSubtitlingDescriptor->astSubtitling_Info[iLoopCount].stISO_639_Language_code.aucPrivate_data_byte[0] = pucDescriptorBuffer[2 + iDescriptorPosition + iLoopCount * iOneInfoLength];
		pstSubtitlingDescriptor->astSubtitling_Info[iLoopCount].stISO_639_Language_code.aucPrivate_data_byte[1] = pucDescriptorBuffer[3 + iDescriptorPosition + iLoopCount * iOneInfoLength];
		pstSubtitlingDescriptor->astSubtitling_Info[iLoopCount].stISO_639_Language_code.aucPrivate_data_byte[2] = pucDescriptorBuffer[4 + iDescriptorPosition + iLoopCount * iOneInfoLength];
		pstSubtitlingDescriptor->astSubtitling_Info[iLoopCount].uiSubtitling_type = pucDescriptorBuffer[5 + iDescriptorPosition + iLoopCount * iOneInfoLength];
		pstSubtitlingDescriptor->astSubtitling_Info[iLoopCount].uiComposition_page_id = (pucDescriptorBuffer[6 + iDescriptorPosition + iLoopCount * iOneInfoLength] << 8) | pucDescriptorBuffer[7 + iDescriptorPosition + iLoopCount * iOneInfoLength];
		pstSubtitlingDescriptor->astSubtitling_Info[iLoopCount].uiAncillary_page_id = (pucDescriptorBuffer[8 + iDescriptorPosition + iLoopCount * iOneInfoLength] << 8) | pucDescriptorBuffer[9 + iDescriptorPosition + iLoopCount * iOneInfoLength];
	}
	return iDescriptorPosition;
}

/**********************************************************
 *
 * 解析Terrestrial Delivery System Descriptor描述子信息
 *
 **********************************************************/
int GetTerrestrialDeliverySystemDescriptor(TERRESTRIAL_DELIVERY_SYSTEM_DESCRIPTOR_T *pstTerrestrialDeliverySystemDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition)
{
	memset(pstTerrestrialDeliverySystemDescriptor, 0, sizeof(TERRESTRIAL_DELIVERY_SYSTEM_DESCRIPTOR_T));
	pstTerrestrialDeliverySystemDescriptor->uiDescriptor_tag = pucDescriptorBuffer[iDescriptorPosition];
	pstTerrestrialDeliverySystemDescriptor->uiDescriptor_length = pucDescriptorBuffer[1 + iDescriptorPosition];
	if (pstTerrestrialDeliverySystemDescriptor->uiDescriptor_length > 0)
	{
		pstTerrestrialDeliverySystemDescriptor->uiCentre_frequency = (pucDescriptorBuffer[2 + iDescriptorPosition] << 24) | (pucDescriptorBuffer[3 + iDescriptorPosition] << 16) | (pucDescriptorBuffer[4 + iDescriptorPosition] << 8)
				| (pucDescriptorBuffer[5 + iDescriptorPosition]);
		pstTerrestrialDeliverySystemDescriptor->uiBandwidth = pucDescriptorBuffer[6 + iDescriptorPosition] >> 5;
		pstTerrestrialDeliverySystemDescriptor->uiPriority = (pucDescriptorBuffer[6 + iDescriptorPosition] >> 4) & 0x01;
		pstTerrestrialDeliverySystemDescriptor->uiTime_Slicing_indicator = (pucDescriptorBuffer[6 + iDescriptorPosition] >> 3) & 0x01;
		pstTerrestrialDeliverySystemDescriptor->uiMPE_FEC_indicator = (pucDescriptorBuffer[6 + iDescriptorPosition] >> 2) & 0x01;
		pstTerrestrialDeliverySystemDescriptor->uiReserved_future_use_first = pucDescriptorBuffer[6 + iDescriptorPosition] & 0x03;
		pstTerrestrialDeliverySystemDescriptor->uiConstellation = pucDescriptorBuffer[7 + iDescriptorPosition] >> 6;
		pstTerrestrialDeliverySystemDescriptor->uiHierarchy_information = (pucDescriptorBuffer[7 + iDescriptorPosition] >> 3) & 0x07;
		pstTerrestrialDeliverySystemDescriptor->uiCode_rate_HP_stream = pucDescriptorBuffer[7 + iDescriptorPosition] & 0x07;
		pstTerrestrialDeliverySystemDescriptor->uiCode_rate_LP_stream = pucDescriptorBuffer[8 + iDescriptorPosition] >> 5;
		pstTerrestrialDeliverySystemDescriptor->uiGuard_interval = (pucDescriptorBuffer[8 + iDescriptorPosition] >> 3) & 0x03;
		pstTerrestrialDeliverySystemDescriptor->uiTransmission_mode = (pucDescriptorBuffer[8 + iDescriptorPosition] >> 1) & 0x03;
		pstTerrestrialDeliverySystemDescriptor->uiOther_frequency_flag = pucDescriptorBuffer[8 + iDescriptorPosition] >> 7;
		pstTerrestrialDeliverySystemDescriptor->uiReserved_future_use_second = (pucDescriptorBuffer[9 + iDescriptorPosition] << 24) | (pucDescriptorBuffer[10 + iDescriptorPosition] << 16) | (pucDescriptorBuffer[11 + iDescriptorPosition] << 8)
				| (pucDescriptorBuffer[12 + iDescriptorPosition]);
	}
	else
	{
	}
	return iDescriptorPosition;
}

/******************************************
 *
 * 解析Short Event Descriptor描述子信息
 *
 ******************************************/
int GetShortEventDescriptor(SHORT_EVENT_DESCRIPTOR_T *pstShortEventDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition)
{
	memset(pstShortEventDescriptor, 0, sizeof(SHORT_EVENT_DESCRIPTOR_T));
	pstShortEventDescriptor->uiDescriptor_tag = pucDescriptorBuffer[iDescriptorPosition];
	pstShortEventDescriptor->uiDescriptor_length = pucDescriptorBuffer[1 + iDescriptorPosition];
	pstShortEventDescriptor->stISO_639_Language_code.aucPrivate_data_byte[0] = pucDescriptorBuffer[2 + iDescriptorPosition];
	pstShortEventDescriptor->stISO_639_Language_code.aucPrivate_data_byte[1] = pucDescriptorBuffer[3 + iDescriptorPosition];
	pstShortEventDescriptor->stISO_639_Language_code.aucPrivate_data_byte[2] = pucDescriptorBuffer[4 + iDescriptorPosition];
	pstShortEventDescriptor->uiEvent_name_length = pucDescriptorBuffer[5 + iDescriptorPosition];
	if (pstShortEventDescriptor->uiEvent_name_length > 0)
	{
		memcpy(pstShortEventDescriptor->aucEvent_name_char, pucDescriptorBuffer + 6, pstShortEventDescriptor->uiEvent_name_length);
	}
	else
	{
	}
	pstShortEventDescriptor->uiText_length = pucDescriptorBuffer[6 + iDescriptorPosition + pstShortEventDescriptor->uiEvent_name_length];
	if (pstShortEventDescriptor->uiText_length > 0)
	{
		memcpy(pstShortEventDescriptor->aucEvent_name_char, pucDescriptorBuffer + 7 + pstShortEventDescriptor->uiEvent_name_length, pstShortEventDescriptor->uiText_length);
	}
	else
	{
	}
	return iDescriptorPosition;
}

/******************************************
 *
 * 解析Extended Event Descriptor描述子信息
 *
 ******************************************/
int GetExtendedEventDescriptor(EXTENDED_EVENT_DESCRIPTOR_T *pstExtendedEventDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition)
{
	int iLoopCount = 0;
	int iDescriptorInfoPostion = 0;
	memset(pstExtendedEventDescriptor, 0, sizeof(EXTENDED_EVENT_DESCRIPTOR_T));
	pstExtendedEventDescriptor->uiDescriptor_tag = pucDescriptorBuffer[iDescriptorPosition];
	pstExtendedEventDescriptor->uiDescriptor_length = pucDescriptorBuffer[1 + iDescriptorPosition];
	pstExtendedEventDescriptor->uiDescriptor_number = pucDescriptorBuffer[2 + iDescriptorPosition] >> 4;
	pstExtendedEventDescriptor->uiLast_descriptor_number = pucDescriptorBuffer[2 + iDescriptorPosition] & 0x0f;
	pstExtendedEventDescriptor->stISO_639_language_code.aucPrivate_data_byte[0] = pucDescriptorBuffer[3 + iDescriptorPosition];
	pstExtendedEventDescriptor->stISO_639_language_code.aucPrivate_data_byte[1] = pucDescriptorBuffer[4 + iDescriptorPosition];
	pstExtendedEventDescriptor->stISO_639_language_code.aucPrivate_data_byte[2] = pucDescriptorBuffer[5 + iDescriptorPosition];
	pstExtendedEventDescriptor->uiLength_of_items = pucDescriptorBuffer[6 + iDescriptorPosition];
	if (pstExtendedEventDescriptor->uiLength_of_items > 0)
	{
		for (iDescriptorInfoPostion = 0; iDescriptorInfoPostion < pstExtendedEventDescriptor->uiLength_of_items; iDescriptorInfoPostion += 2)
		{
			pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].uiItem_descriptor_length = pucDescriptorBuffer[7 + iDescriptorPosition];
			if (pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].uiItem_descriptor_length > 0)
			{
				memcpy(pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].aucItem_descriptor_char, pucDescriptorBuffer + 8, pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].uiItem_descriptor_length);
			}
			else
			{
			}
			iDescriptorInfoPostion += pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].uiItem_descriptor_length;

			pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].uiItem_length = pucDescriptorBuffer[8 + iDescriptorPosition];
			if (pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].uiItem_length > 0)
			{
				memcpy(pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].aucItem_descriptor_char, pucDescriptorBuffer + 9 + iDescriptorInfoPostion, pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].uiItem_length);
			}
			else
			{
			}
			iDescriptorInfoPostion += pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].uiItem_length;
			iLoopCount++;
		}
	}
	else
	{
	}
	pstExtendedEventDescriptor->uiText_length = pucDescriptorBuffer[7 + iDescriptorPosition + pstExtendedEventDescriptor->uiLength_of_items];
	if (pstExtendedEventDescriptor->uiText_length > 0)
	{
		memcpy(pstExtendedEventDescriptor->aucText_char, pucDescriptorBuffer + 8 + pstExtendedEventDescriptor->uiLength_of_items, pstExtendedEventDescriptor->uiText_length);
	}
	else
	{
	}
	return iDescriptorPosition;
}

/*********************************************************
 *
 * 解析Frequency List Descriptor描述子信息
 *
 *********************************************************/
int GetFrequencyListDescriptor(FREQUENCY_LIST_DESCRIPTOR_T *pstFrequencyListDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition)
{
	int iLoopCount = 0;
	int iLoopInfoLength = 4;
	memset(pstFrequencyListDescriptor, 0, sizeof(FREQUENCY_LIST_DESCRIPTOR_T));
	pstFrequencyListDescriptor->uiDescriptor_tag = pucDescriptorBuffer[iDescriptorPosition];
	pstFrequencyListDescriptor->uiDescriptor_length = pucDescriptorBuffer[1 + iDescriptorPosition];
	pstFrequencyListDescriptor->uiReserved_future_use = pucDescriptorBuffer[2 + iDescriptorPosition] >> 2;
	pstFrequencyListDescriptor->uiCoding_type = pucDescriptorBuffer[2 + iDescriptorPosition] & 0x03;
	for (iLoopCount = 0; iLoopCount * iLoopInfoLength < pstFrequencyListDescriptor->uiDescriptor_length - 1; iLoopCount++)
	{
		pstFrequencyListDescriptor->astCentre_frequency[iLoopCount].uiCentre_frequency = (pucDescriptorBuffer[3 + iDescriptorPosition + iLoopCount * iLoopInfoLength] << 24)
				| (pucDescriptorBuffer[4 + iDescriptorPosition + iLoopCount * iLoopInfoLength] << 16) | (pucDescriptorBuffer[5 + iDescriptorPosition + iLoopCount * iLoopInfoLength] << 8)
				| (pucDescriptorBuffer[6 + iDescriptorPosition + iLoopCount * iLoopInfoLength]);
	}
	return iDescriptorPosition;
}

// 13818描述子

/******************************************
 *
 * 解析Video Stream Descriptor描述子信息
 *
 ******************************************/
int GetVideoStreamDescriptor(VIDEO_STREAM_DESCRIPTOR_T *pstVideoStreamDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition)
{
	memset(pstVideoStreamDescriptor, 0, sizeof(VIDEO_STREAM_DESCRIPTOR_T));
	pstVideoStreamDescriptor->uiDescriptor_tag = pucDescriptorBuffer[iDescriptorPosition + 0];
	pstVideoStreamDescriptor->uiDescriptor_length = pucDescriptorBuffer[iDescriptorPosition + 1];
	pstVideoStreamDescriptor->uiMultiple_frame_rate_flag = pucDescriptorBuffer[iDescriptorPosition + 2] >> 7;
	pstVideoStreamDescriptor->uiFrame_rate_code = (pucDescriptorBuffer[iDescriptorPosition + 2] >> 3) & 0x0f;
	pstVideoStreamDescriptor->uiMPEG_2_flag = (pucDescriptorBuffer[iDescriptorPosition + 2] >> 2) & 0x01;
	pstVideoStreamDescriptor->uiConstrained_parameter_flag = (pucDescriptorBuffer[iDescriptorPosition + 2] >> 1) & 0x01;
	pstVideoStreamDescriptor->uiStill_picture_flag = pucDescriptorBuffer[iDescriptorPosition + 2] & 0x01;

	if (1 == pstVideoStreamDescriptor->uiMPEG_2_flag)
	{
		pstVideoStreamDescriptor->uiProfile_and_level_indication = pucDescriptorBuffer[iDescriptorPosition + 3];
		pstVideoStreamDescriptor->uiChroma_format = pucDescriptorBuffer[iDescriptorPosition + 4] >> 6;
		pstVideoStreamDescriptor->uiFrame_rate_extension_flag = (pucDescriptorBuffer[iDescriptorPosition + 4] >> 5) & 0x01;
		pstVideoStreamDescriptor->uiReserved = pucDescriptorBuffer[iDescriptorPosition + 4] & 0x1f;
	}
	else
	{
	}
	return 1;
}

/******************************************
 *
 * 解析Audio Stream Descriptor描述子信息
 *
 ******************************************/
int GetAudioStreamDescriptor(AUDIO_STREAM_DESCRIPTOR_T *pstAudioStreamDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition)
{
	memset(pstAudioStreamDescriptor, 0, sizeof(VIDEO_STREAM_DESCRIPTOR_T));
	pstAudioStreamDescriptor->uiDescriptor_tag = pucDescriptorBuffer[iDescriptorPosition + 0];
	pstAudioStreamDescriptor->uiDescriptor_length = pucDescriptorBuffer[iDescriptorPosition + 1];
	pstAudioStreamDescriptor->uiFree_format_flag = pucDescriptorBuffer[iDescriptorPosition + 2] >> 7;
	pstAudioStreamDescriptor->uiID = (pucDescriptorBuffer[iDescriptorPosition + 2] >> 6) & 0x01;
	pstAudioStreamDescriptor->uiLayer = (pucDescriptorBuffer[iDescriptorPosition + 2] >> 4) & 0x03;
	pstAudioStreamDescriptor->uiReserved = pucDescriptorBuffer[iDescriptorPosition + 2] & 0x0f;
	return iDescriptorPosition;
}

/******************************************
 *
 * 解析Data Stream Alignment Descriptor描述子信息
 *
 ******************************************/
int GetDataStreamAlignmentDescriptor(DATA_STREAM_ALIGNMENT_DESCRIPTOR_T *pstDataStreamAlignmentDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition)
{
	memset(pstDataStreamAlignmentDescriptor, 0, sizeof(DATA_STREAM_ALIGNMENT_DESCRIPTOR_T));
	pstDataStreamAlignmentDescriptor->uiDescriptor_tag = pucDescriptorBuffer[iDescriptorPosition + 0];
	pstDataStreamAlignmentDescriptor->uiDescriptor_length = pucDescriptorBuffer[iDescriptorPosition + 1];
	pstDataStreamAlignmentDescriptor->uiAlignment_type = pucDescriptorBuffer[iDescriptorPosition + 2];
	return iDescriptorPosition;
}

/******************************************
 *
 * 解析Service   Descriptor描述子信息
 *
 ******************************************/
int GetMaximumBitrateDescriptor(MAXIMUM_BITRATE_DESCRIPTOR_T *pstMaximumBitrateDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition)
{
	memset(pstMaximumBitrateDescriptor, 0, sizeof(MAXIMUM_BITRATE_DESCRIPTOR_T));
	pstMaximumBitrateDescriptor->uiDescriptor_tag = pucDescriptorBuffer[iDescriptorPosition];
	pstMaximumBitrateDescriptor->uiDescriptor_length = pucDescriptorBuffer[1 + iDescriptorPosition];
	pstMaximumBitrateDescriptor->uiReserved = pucDescriptorBuffer[2 + iDescriptorPosition] >> 6;
	pstMaximumBitrateDescriptor->uiMaximum_bitrate = ((pucDescriptorBuffer[2 + iDescriptorPosition] & 0x3f) << 16) | (pucDescriptorBuffer[3 + iDescriptorPosition] << 8) | (pucDescriptorBuffer[4 + iDescriptorPosition]);
	return iDescriptorPosition;
}

/******************************************
 *
 * 解析CA   Descriptor描述子信息
 *
 ******************************************/
int GetCA_Descriptor(CA_DESCRIPTOR_T *pstCA_Descriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition)
{
	memset(pstCA_Descriptor, 0, sizeof(CA_DESCRIPTOR_T));
	pstCA_Descriptor->uiDescriptor_tag = pucDescriptorBuffer[iDescriptorPosition + 0];
	pstCA_Descriptor->uiDescriptor_length = pucDescriptorBuffer[iDescriptorPosition + 1];
	pstCA_Descriptor->uiCA_system_ID = (pucDescriptorBuffer[iDescriptorPosition + 2] << 8) | pucDescriptorBuffer[iDescriptorPosition + 3];
	pstCA_Descriptor->uiReserved = pucDescriptorBuffer[iDescriptorPosition + 4] >> 5;
	pstCA_Descriptor->uiCA_PID = ((pucDescriptorBuffer[iDescriptorPosition + 4] & 0x1f) << 8) | pucDescriptorBuffer[iDescriptorPosition + 5];
	memcpy(pstCA_Descriptor->aucPrivate_data_byte, pucDescriptorBuffer + iDescriptorPosition + 6, pstCA_Descriptor->uiDescriptor_length - 4);
	return iDescriptorPosition;
}

/******************************************
 *
 * 解析System Clock Descriptor描述子信息
 *
 ******************************************/
int GetSystemClockDescriptor(SYSTEM_CLOCK_DESCRIPTOR_T *pstSystemClockDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition)
{
	memset(pstSystemClockDescriptor, 0, sizeof(SYSTEM_CLOCK_DESCRIPTOR_T));
	pstSystemClockDescriptor->uiDescriptor_tag = pucDescriptorBuffer[iDescriptorPosition];
	pstSystemClockDescriptor->uiDescriptor_length = pucDescriptorBuffer[1 + iDescriptorPosition];
	pstSystemClockDescriptor->uiExternal_clock_reference_indicator = pucDescriptorBuffer[2 + iDescriptorPosition] >> 7;
	pstSystemClockDescriptor->uiReserved_first = (pucDescriptorBuffer[2 + iDescriptorPosition] >> 6) & 0x01;
	pstSystemClockDescriptor->uiClock_accuracy_integer = pucDescriptorBuffer[2 + iDescriptorPosition] & 0x3f;
	pstSystemClockDescriptor->uiClock_accuracy_exponent = pucDescriptorBuffer[3 + iDescriptorPosition] >> 5;
	pstSystemClockDescriptor->uiReserved_second = pucDescriptorBuffer[3 + iDescriptorPosition] & 0x1f;
	return iDescriptorPosition;
}

/******************************************
 *
 * 解析ISO_639_Language_Descriptor描述子信息
 *
 ******************************************/
int GetISO_639_Language_Descriptor(ISO_639_LANGUAGE_DESCRIPTOR_T *pstISO_639_LanguageDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition)
{
	int iDescriptorLength = 0;
	int iDescriptorLoopCount = 0;
	memset(pstISO_639_LanguageDescriptor, 0, sizeof(ISO_639_LANGUAGE_DESCRIPTOR_T));
	pstISO_639_LanguageDescriptor->uiDescriptor_tag = pucDescriptorBuffer[iDescriptorPosition + 0];
	pstISO_639_LanguageDescriptor->uiDescriptor_length = pucDescriptorBuffer[iDescriptorPosition + 1];
	for (iDescriptorLength = 0; iDescriptorLength < pstISO_639_LanguageDescriptor->uiDescriptor_length - 1; iDescriptorLength += 3)
	{
		pstISO_639_LanguageDescriptor->stISO_639_Language_code[iDescriptorLoopCount].aucPrivate_data_byte[0] = pucDescriptorBuffer[iDescriptorPosition + 2 + (iDescriptorLoopCount * 3)];
		pstISO_639_LanguageDescriptor->stISO_639_Language_code[iDescriptorLoopCount].aucPrivate_data_byte[1] = pucDescriptorBuffer[iDescriptorPosition + 3 + (iDescriptorLoopCount * 3)];
		pstISO_639_LanguageDescriptor->stISO_639_Language_code[iDescriptorLoopCount].aucPrivate_data_byte[2] = pucDescriptorBuffer[iDescriptorPosition + 4 + (iDescriptorLoopCount * 3)];
		iDescriptorLoopCount++;
	}
	pstISO_639_LanguageDescriptor->uiAudio_type = pucDescriptorBuffer[iDescriptorPosition + 2 + pstISO_639_LanguageDescriptor->uiDescriptor_length - 1];
	return iDescriptorPosition;
}

