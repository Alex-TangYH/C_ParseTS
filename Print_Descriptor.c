#include <stdio.h>
#include <string.h>
#include "Parse_Descriptor.h"
#include "Print_Descriptor.h"


/*****************************************************************
 *
 * 打印Network Name Descriptor描述子
 *
 *****************************************************************/
void Print_NetworkNameDescriptor(NETWORK_NAME_DESCRIPTOR_T *pstNetworkNameDescriptor, char *pacOutputPrefix)
{
	printf("%sNetworkNameDescriptor:\n", pacOutputPrefix);
	printf("%s├─NetworkNameDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstNetworkNameDescriptor->uiDescriptor_tag);
	if (pstNetworkNameDescriptor->uiDescriptor_length > 0)
	{
		printf("%s├─NetworkNameDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstNetworkNameDescriptor->uiDescriptor_length);
		printf("%s└─NetworkNameDescriptor.NewworkName: %s\n", pacOutputPrefix, pstNetworkNameDescriptor->aucInfo);
	}
	else
	{
		printf("%s└─NetworkNameDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstNetworkNameDescriptor->uiDescriptor_length);
	}

}

/*****************************************************************
 *
 * 打印Bouquet Name Descriptor描述子
 *
 *****************************************************************/

void Print_BouquetNameDescriptor(BOUQUET_NAME_DESCRIPTOR_T *pstBouquetNameDescriptor, char *pacOutputPrefix)
{
	printf("%sBouquetNameDescriptor:\n", pacOutputPrefix);
	printf("%s├─BouquetNameDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstBouquetNameDescriptor->uiDescriptor_tag);
	
	if (pstBouquetNameDescriptor->uiDescriptor_length > 0)
	{
		printf("%s├─BouquetNameDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstBouquetNameDescriptor->uiDescriptor_length);
		printf("%s└─BouquetNameDescriptor.Info: %s\n", pacOutputPrefix, pstBouquetNameDescriptor->aucInfo);
	}
	else
	{
		printf("%s└─BouquetNameDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstBouquetNameDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印Service List Descriptorr描述子
 *
 *****************************************************************/

void Print_ServiceListDescriptor(SERVICE_LIST_DESCRIPTOR_T *pstServiceListDescriptor, char *pacOutputPrefix)
{
	int iDescriptorInfoCount = 0;
	int iDescriptorInfoPosition = 0;
	
	printf("%sServiceListDescriptor:\n", pacOutputPrefix);
	printf("%s├─ServiceListDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstServiceListDescriptor->uiDescriptor_tag);
	
	if (pstServiceListDescriptor->uiDescriptor_length > 0)
	{
		printf("%s├─ServiceListDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstServiceListDescriptor->uiDescriptor_length);
		for (iDescriptorInfoPosition = 0; iDescriptorInfoPosition < pstServiceListDescriptor->uiDescriptor_length; iDescriptorInfoPosition += 3)
		{
			
			if (pstServiceListDescriptor->uiDescriptor_length - iDescriptorInfoPosition > 2)
			{
				printf("%s├─ServiceListDescriptor.Service_info[%d].Serviec_id: 0x%02x\n", pacOutputPrefix, iDescriptorInfoCount, pstServiceListDescriptor->astService_info[iDescriptorInfoCount].uiServiec_id);
				printf("%s└─ServiceListDescriptor.Service_info[%d].Service_type: 0x%02x\n", pacOutputPrefix, iDescriptorInfoCount, pstServiceListDescriptor->astService_info[iDescriptorInfoCount].uiService_type);
			}
			else
			{
				printf("%s└─ServiceListDescriptor.Service_info[%d].Serviec_id: 0x%02x\n", pacOutputPrefix, iDescriptorInfoCount, pstServiceListDescriptor->astService_info[iDescriptorInfoCount].uiServiec_id);
			}
			iDescriptorInfoCount++;
		}
	}
	else
	{
		printf("%s└─ServiceListDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstServiceListDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印Satellite Delivery System Descriptor描述子
 *
 *****************************************************************/
void Print_SatelliteDeliverySystemDescriptor(SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR_T *pstSatelliteDeliverySystemDescriptor, char *pacOutputPrefix)
{
	printf("%sSatelliteDeliverySystemDescriptor:\n", pacOutputPrefix);
	printf("%s├─SatelliteDeliverySystemDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiDescriptor_tag);
	if (pstSatelliteDeliverySystemDescriptor->uiDescriptor_length > 0)
	{
		printf("%s├─SatelliteDeliverySystemDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiDescriptor_length);
		printf("%s├─SatelliteDeliverySystemDescriptor.Frequency: 0x%08x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiFrequency);
		printf("%s├─SatelliteDeliverySystemDescriptor.Orbital_position: 0x%04x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiOrbital_position);
		printf("%s├─SatelliteDeliverySystemDescriptor.West_east_flag: 0x%02x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiWest_east_flag);
		printf("%s├─SatelliteDeliverySystemDescriptor.Polarization: 0x%02x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiPolarization);
		printf("%s├─SatelliteDeliverySystemDescriptor.Roll_off: 0x%02x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiRoll_off);
		printf("%s├─SatelliteDeliverySystemDescriptor.Modulation_system: 0x%02x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiModulation_system);
		printf("%s├─SatelliteDeliverySystemDescriptor.Modulation_type: 0x%02x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiModulation_type);
		printf("%s├─SatelliteDeliverySystemDescriptor.Symbol_rate: 0x%02x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiSymbol_rate);
		printf("%s└─SatelliteDeliverySystemDescriptor.FEC_inner: 0x%02x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiFEC_inner);
	}
	else
	{
		printf("%s└─SatelliteDeliverySystemDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印Terrestrial Delivery System Descriptor描述子
 *
 *****************************************************************/
void Print_TerrestrialDeliverySystemDescriptor(TERRESTRIAL_DELIVERY_SYSTEM_DESCRIPTOR_T *pstTerrestrialDeliverySystemDescriptor, char *pacOutputPrefix)
{
	printf("%sTerrestrialDeliverySystemDescriptor:\n", pacOutputPrefix);
	printf("%s├─TerrestrialDeliverySystemDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiDescriptor_tag);
	if (pstTerrestrialDeliverySystemDescriptor->uiDescriptor_length > 0)
	{
		printf("%s├─TerrestrialDeliverySystemDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiDescriptor_length);
		printf("%s├─TerrestrialDeliverySystemDescriptor.Centre_frequency: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiCentre_frequency);
		printf("%s├─TerrestrialDeliverySystemDescriptor.Bandwidth: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiBandwidth);
		printf("%s├─TerrestrialDeliverySystemDescriptor.Priority: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiPriority);
		printf("%s├─TerrestrialDeliverySystemDescriptor.Time_Slicing_indicator: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiTime_Slicing_indicator);
		printf("%s├─TerrestrialDeliverySystemDescriptor.MPE_FEC_indicator: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiMPE_FEC_indicator);
		printf("%s├─TerrestrialDeliverySystemDescriptor.Reserved_future_use_first: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiReserved_future_use_first);
		printf("%s├─TerrestrialDeliverySystemDescriptor.Constellation: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiConstellation);
		printf("%s├─TerrestrialDeliverySystemDescriptor.Hierarchy_information: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiHierarchy_information);
		printf("%s├─TerrestrialDeliverySystemDescriptor.Code_rate_HP_stream: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiCode_rate_HP_stream);
		printf("%s├─TerrestrialDeliverySystemDescriptor.Code_rate_LP_stream: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiCode_rate_LP_stream);
		printf("%s├─TerrestrialDeliverySystemDescriptor.Guard_interval: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiGuard_interval);
		printf("%s├─TerrestrialDeliverySystemDescriptor.Transmission_mode: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiTransmission_mode);
		printf("%s├─TerrestrialDeliverySystemDescriptor.Other_frequency_flag: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiOther_frequency_flag);
		printf("%s└─TerrestrialDeliverySystemDescriptor.uiReserved_future_use_second: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiReserved_future_use_second);
	}
	else
	{
		printf("%s└─TerrestrialDeliverySystemDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印Cable Delivery System Descriptor描述子
 *
 *****************************************************************/
void Print_CableDeliverySystemDescriptor(CABLE_DELIVERY_SYSTEM_DESCRIPTOR_T *pstCableDeliverySystemDescriptor, char *pacOutputPrefix)
{
	printf("%sCableDeliverySystemDescriptor:\n", pacOutputPrefix);
	printf("%s├─CableDeliverySystemDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstCableDeliverySystemDescriptor->uiDescriptor_tag);
	if (pstCableDeliverySystemDescriptor->uiDescriptor_length > 0)
	{
		printf("%s├─CableDeliverySystemDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstCableDeliverySystemDescriptor->uiDescriptor_length);
		printf("%s├─CableDeliverySystemDescriptor.Frequency: 0x%02x\n", pacOutputPrefix, pstCableDeliverySystemDescriptor->uiFrequency);
		printf("%s├─CableDeliverySystemDescriptor.Reserved_future_use: 0x%02x\n", pacOutputPrefix, pstCableDeliverySystemDescriptor->uiReserved_future_use);
		printf("%s├─CableDeliverySystemDescriptor.Fec_outer: 0x%02x\n", pacOutputPrefix, pstCableDeliverySystemDescriptor->uiFec_outer);
		printf("%s├─CableDeliverySystemDescriptor.Modulation: 0x%02x\n", pacOutputPrefix, pstCableDeliverySystemDescriptor->uiModulation);
		printf("%s├─CableDeliverySystemDescriptor.Symbol_rate: 0x%02x\n", pacOutputPrefix, pstCableDeliverySystemDescriptor->uiSymbol_rate);
		printf("%s└─CableDeliverySystemDescriptor.FEC_inner: 0x%02x\n", pacOutputPrefix, pstCableDeliverySystemDescriptor->uiFEC_inner);
	}
	else
	{
		printf("%s└─CableDeliverySystemDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstCableDeliverySystemDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印ServiceDescriptor描述子
 *
 *****************************************************************/
void Print_ServiceDescriptor(SERVICE_DESCRIPTOR_T *pstServiceDescriptor, char *pacOutputPrefix)
{
	printf("%sServiceDescriptor:\n", pacOutputPrefix);
	printf("%s├─ServiceDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstServiceDescriptor->uiDescriptor_tag);
	if (pstServiceDescriptor->uiDescriptor_length > 0)
	{
		printf("%s├─ServiceDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstServiceDescriptor->uiDescriptor_length);
		printf("%s├─ServiceDescriptor.Service_type: 0x%02x\n", pacOutputPrefix, pstServiceDescriptor->uiService_type);
		printf("%s├─ServiceDescriptor.Service_provider_name_length: 0x%02x\n", pacOutputPrefix, pstServiceDescriptor->uiService_provider_name_length);
		if (pstServiceDescriptor->uiService_provider_name_length > 0)
		{
			printf("%s├─ServiceDescriptor.Service_provider_name: %s\n", pacOutputPrefix, pstServiceDescriptor->aucService_provider_name);
		}
		else
		{
		}
		
		if (pstServiceDescriptor->uiService_provider_name_length > 0)
		{
			printf("%s├─ServiceDescriptor.Service_name_length: 0x%02x\n", pacOutputPrefix, pstServiceDescriptor->uiService_name_length);
			printf("%s└─ServiceDescriptor.Service_name: %s\n", pacOutputPrefix, pstServiceDescriptor->aucService_name);
		}
		else
		{
			printf("%s└─ServiceDescriptor.Service_name_length: 0x%02x\n", pacOutputPrefix, pstServiceDescriptor->uiService_name_length);
		}
	}
	else
	{
		printf("%s└─ServiceDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstServiceDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印LinkageDescriptor描述子
 *
 *****************************************************************/
void Print_LinkageDescriptor(LINKAGE_DESCRIPTOR_T *pstLinkageDescriptor, char *pacOutputPrefix)
{
	printf("%sLinkageDescriptor:\n", pacOutputPrefix);
	printf("%s├─LinkageDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->uiDescriptor_tag);
	if (pstLinkageDescriptor->uiDescriptor_length > 0)
	{
		printf("%s├─LinkageDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->uiDescriptor_length);
		printf("%s├─LinkageDescriptor.Transport_stream_id: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->uiTransport_stream_id);
		printf("%s├─LinkageDescriptor.Original_network_id: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->uiOriginal_network_id);
		printf("%s├─LinkageDescriptor.Service_id: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->uiService_id);
		printf("%s├─LinkageDescriptor.Linage_type: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->uiLinage_type);
		if (0x08 == pstLinkageDescriptor->uiLinage_type)
		{
			printf("%s├─LinkageDescriptor.Hand_over_type: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiHand_over_type);
			printf("%s├─LinkageDescriptor.Reserved_future_use: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiReserved_future_use);
			printf("%s├─LinkageDescriptor.Origin_type: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiOrigin_type);
			if ((1 == pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiHand_over_type) || (2 == pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiHand_over_type)
					|| (3 == pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiHand_over_type))
			{
				printf("%s├─LinkageDescriptor.Network_id: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiNetwork_id);
			}
			if (0 == pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiOrigin_type)
			{
				printf("%s├─LinkageDescriptor.Initial_service_id: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiInitial_service_id);
			}
		}
		else if (0x0D == pstLinkageDescriptor->uiLinage_type)
		{
			printf("%s├─LinkageDescriptor.Target_event_id: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_0d.uiTarget_event_id);
			printf("%s├─LinkageDescriptor.Target_listed: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_0d.uiTarget_listed);
			printf("%s├─LinkageDescriptor.Event_simulcast: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_0d.uiEvent_simulcast);
			printf("%s├─LinkageDescriptor.Reserved: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_0d.uiReserved);
		}
		else
		{
		}
		printf("%s└─LinkageDescriptor.Private_data: %s\n", pacOutputPrefix, pstLinkageDescriptor->aucPrivate_data);
	}
	else
	{
		printf("%s└─LinkageDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印Stream Indentifier Descriptor描述子
 *
 *****************************************************************/
void Print_StreamIndentifierDescriptor(STREAM_IDENTIFIER_DESCRIPTOR_T *pstStreamIndentifierDescriptor, char *pacOutputPrefix)
{
	printf("%sStreamIndentifierDescriptor:\n", pacOutputPrefix);
	printf("%s├─StreamIndentifierDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstStreamIndentifierDescriptor->uiDescriptor_tag);
	if (pstStreamIndentifierDescriptor->uiDescriptor_length > 0)
	{
		printf("%s├─StreamIndentifierDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstStreamIndentifierDescriptor->uiDescriptor_length);
		printf("%s└─StreamIndentifierDescriptor.Component_tag: 0x%02x\n", pacOutputPrefix, pstStreamIndentifierDescriptor->uiComponent_tag);
	}
	else
	{
		printf("%s└─StreamIndentifierDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstStreamIndentifierDescriptor->uiDescriptor_length);
	}
}

//13818

/*****************************************************************
 *
 * 打印VideoStreamDescriptor描述子
 *
 *****************************************************************/
void Print_VideoStreamDescriptor(VIDEO_STREAM_DESCRIPTOR_T *pstVideoStreamDescriptor, char *pacOutputPrefix)
{
	printf("%sVideoStreamDescriptor:\n", pacOutputPrefix);
	printf("%s├─VideoStreamDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiDescriptor_tag);
	if (pstVideoStreamDescriptor->uiDescriptor_length > 0)
	{
		printf("%s├─VideoStreamDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiDescriptor_length);
		printf("%s├─VideoStreamDescriptor.uiMultiple_frame_rate_flag: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiMultiple_frame_rate_flag);
		printf("%s├─VideoStreamDescriptor.uiFrame_rate_code: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiFrame_rate_code);
		printf("%s├─VideoStreamDescriptor.uiMPEG_2_flag: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiMPEG_2_flag);
		printf("%s├─VideoStreamDescriptor.uiConstrained_parameter_flag: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiConstrained_parameter_flag);
		
		if (1 == pstVideoStreamDescriptor->uiMPEG_2_flag)
		{
			printf("%s├─VideoStreamDescriptor.uiStill_picture_flag: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiStill_picture_flag);
			printf("%s├─VideoStreamDescriptor.uiProfile_and_level_indication: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiProfile_and_level_indication);
			printf("%s├─VideoStreamDescriptor.uiChroma_format: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiChroma_format);
			printf("%s├─VideoStreamDescriptor.uiFrame_rate_extension_flag: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiFrame_rate_extension_flag);
			printf("%s└─VideoStreamDescriptor.uiReserved: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiReserved);
		}
		else
		{
			printf("%s└─VideoStreamDescriptor.uiStill_picture_flag: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiStill_picture_flag);
		}
		
	}
	else
	{
		printf("%s└─VideoStreamDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiDescriptor_length);
	}

}

/*****************************************************************
 *
 * 打印Audio Stream Descriptor描述子
 *
 *****************************************************************/
void Print_AudioStreamDescriptor(AUDIO_STREAM_DESCRIPTOR_T *pstAudioStreamDescriptor, char *pacOutputPrefix)
{
	printf("%sAudioStreamDescriptor:\n", pacOutputPrefix);
	printf("%s├─AudioStreamDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstAudioStreamDescriptor->uiDescriptor_tag);
	if (pstAudioStreamDescriptor->uiDescriptor_length > 0)
	{
		printf("%s├─AudioStreamDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstAudioStreamDescriptor->uiDescriptor_length);
		printf("%s├─AudioStreamDescriptor.Free_format_flag: 0x%02x\n", pacOutputPrefix, pstAudioStreamDescriptor->uiFree_format_flag);
		printf("%s├─AudioStreamDescriptor.ID: 0x%02x\n", pacOutputPrefix, pstAudioStreamDescriptor->uiID);
		printf("%s├─AudioStreamDescriptor.Layer: 0x%02x\n", pacOutputPrefix, pstAudioStreamDescriptor->uiLayer);
		printf("%s└─AudioStreamDescriptor.Reserved: 0x%02x\n", pacOutputPrefix, pstAudioStreamDescriptor->uiReserved);
	}
	else
	{
		printf("%s└─AudioStreamDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstAudioStreamDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印DataStreamAlignmentDescriptor描述子
 *
 *****************************************************************/
void Print_DataStreamAlignmentDescriptor(DATA_STREAM_ALIGNMENT_DESCRIPTOR_T *pstDataStreamAlignmentDescriptor, char *pacOutputPrefix)
{
	printf("%sDataStreamAlignmentDescriptor:\n", pacOutputPrefix);
	printf("%s├─DataStreamAlignmentDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstDataStreamAlignmentDescriptor->uiDescriptor_tag);
	if (pstDataStreamAlignmentDescriptor->uiDescriptor_length > 0)
	{
		printf("%s├─DataStreamAlignmentDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstDataStreamAlignmentDescriptor->uiDescriptor_length);
		printf("%s└─DataStreamAlignmentDescriptor.Alignment_type: 0x%02x\n", pacOutputPrefix, pstDataStreamAlignmentDescriptor->uiAlignment_type);
	}
	else
	{
		printf("%s└─DataStreamAlignmentDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstDataStreamAlignmentDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印ISO_639_LanguageDescriptor描述子
 *
 *****************************************************************/
void Print_ISO_639_LANGUAGE_DESCRIPTOR(ISO_639_LANGUAGE_DESCRIPTOR_T *pstISO_639_LanguageDescriptor, char *pacOutputPrefix)
{

	int iDescriptorLength = 0;
	int iDescriptorLoopCount = 0;
	
	printf("%sISO_639_LanguageDescriptor:\n", pacOutputPrefix);
	printf("%s├─ISO_639_LanguageDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstISO_639_LanguageDescriptor->uiDescriptor_tag);
	if (pstISO_639_LanguageDescriptor->uiDescriptor_length > 0)
	{
		printf("%s├─ISO_639_LanguageDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstISO_639_LanguageDescriptor->uiDescriptor_length);
		
		for (iDescriptorLength = 0; iDescriptorLength < pstISO_639_LanguageDescriptor->uiDescriptor_length - 1; iDescriptorLength += 3)
		{
			printf("%s├─ISO_639_LanguageDescriptor.Descriptor_length: %s\n", pacOutputPrefix, pstISO_639_LanguageDescriptor->stISO_639_Language_code[iDescriptorLoopCount].aucPrivate_data_byte);
			iDescriptorLoopCount++;
		}
		printf("%s└─ISO_639_LanguageDescriptor.Audio_type: 0x%02x\n", pacOutputPrefix, pstISO_639_LanguageDescriptor->uiAudio_type);
	}
	else
	{
		printf("%s└─ISO_639_LanguageDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstISO_639_LanguageDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印Maximum Bitrate Descriptor描述子
 *
 *****************************************************************/
void Print_MaximumBitrateDescriptor(MAXIMUM_BITRATE_DESCRIPTOR_T *pstMaximumBitrateDescriptor, char *pacOutputPrefix)
{
	printf("%sMaximumBitrateDescriptor:\n", pacOutputPrefix);
	printf("%s├─MaximumBitrateDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstMaximumBitrateDescriptor->uiDescriptor_tag);
	if (pstMaximumBitrateDescriptor->uiDescriptor_length > 0)
	{
		printf("%s├─MaximumBitrateDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstMaximumBitrateDescriptor->uiDescriptor_length);
		printf("%s├─MaximumBitrateDescriptor.Reserved: 0x%02x\n", pacOutputPrefix, pstMaximumBitrateDescriptor->uiReserved);
		printf("%s└─MaximumBitrateDescriptor.Maximum_bitrate: 0x%02x\n", pacOutputPrefix, pstMaximumBitrateDescriptor->uiMaximum_bitrate);
	}
	else
	{
		printf("%s└─MaximumBitrateDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstMaximumBitrateDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印CA_Descriptor描述子
 *
 *****************************************************************/
void Print_CA_Descriptor(CA_DESCRIPTOR_T *pstCA_Descriptor, char *pacOutputPrefix)
{
	printf("%sCA_Descriptor:\n", pacOutputPrefix);
	printf("%s├─CA_Descriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstCA_Descriptor->uiDescriptor_tag);
	if (pstCA_Descriptor->uiDescriptor_length > 0)
	{
		printf("%s├─CA_Descriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstCA_Descriptor->uiDescriptor_length);
		printf("%s├─CA_Descriptor.CA_system_ID: 0x%02x\n", pacOutputPrefix, pstCA_Descriptor->uiCA_system_ID);
		printf("%s├─CA_Descriptor.Reserved: 0x%02x\n", pacOutputPrefix, pstCA_Descriptor->uiReserved);
		if (pstCA_Descriptor->uiDescriptor_length - 4 > 0)
		{
			printf("%s├─CA_Descriptor.CA_PID: 0x%02x\n", pacOutputPrefix, pstCA_Descriptor->uiCA_PID);
			printf("%s└─CA_Descriptor.Private_data_byte: %s\n", pacOutputPrefix, pstCA_Descriptor->aucPrivate_data_byte);
		}
		else
		{
			printf("%s└─CA_Descriptor.CA_PID: 0x%02x\n", pacOutputPrefix, pstCA_Descriptor->uiCA_PID);
		}
		
	}
	else
	{
		printf("%s└─CA_Descriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstCA_Descriptor->uiDescriptor_length);
	}

}

/*****************************************************************
 *
 * 打印System Clock Descriptor描述子
 *
 *****************************************************************/
void Print_SystemClockDescriptor(SYSTEM_CLOCK_DESCRIPTOR_T *pstSystemClockDescriptor, char *pacOutputPrefix)
{
	printf("%sSystemClockDescriptor:\n", pacOutputPrefix);
	printf("%s├─SystemClockDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstSystemClockDescriptor->uiDescriptor_tag);
	if (pstSystemClockDescriptor->uiDescriptor_length > 0)
	{
		printf("%s├─SystemClockDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstSystemClockDescriptor->uiDescriptor_length);
		printf("%s├─SystemClockDescriptor.External_clock_reference_indicator: 0x%02x\n", pacOutputPrefix, pstSystemClockDescriptor->uiExternal_clock_reference_indicator);
		printf("%s├─SystemClockDescriptor.Reserved_first: 0x%02x\n", pacOutputPrefix, pstSystemClockDescriptor->uiReserved_first);
		printf("%s├─SystemClockDescriptor.Clock_accuracy_integer: 0x%02x\n", pacOutputPrefix, pstSystemClockDescriptor->uiClock_accuracy_integer);
		printf("%s├─SystemClockDescriptor.Clock_accuracy_exponent: 0x%02x\n", pacOutputPrefix, pstSystemClockDescriptor->uiClock_accuracy_exponent);
		printf("%s└─SystemClockDescriptor.Reserved_second: 0x%02x\n", pacOutputPrefix, pstSystemClockDescriptor->uiReserved_second);
	}
	else
	{
		printf("%s└─SystemClockDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstSystemClockDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印Teletext Descriptor描述子
 *
 *****************************************************************/
void Print_TeletextDescriptor(TELETEXT_DESCRIPTOR_T *pstTeletextDescriptor, char *pacOutputPrefix)
{
	int iLoopCount = 0;
	printf("%sTeletextDescriptor:\n", pacOutputPrefix);
	printf("%s├─TeletextDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstTeletextDescriptor->uiDescriptor_tag);
	if (pstTeletextDescriptor->uiDescriptor_length > 0)
	{
		printf("%s├─TeletextDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstTeletextDescriptor->uiDescriptor_length);
		for (iLoopCount = 0; iLoopCount * 5 < pstTeletextDescriptor->uiDescriptor_length; iLoopCount++)
		{
			printf("%s├─TeletextDescriptor.Teletext[%d].ISO_639_Language_code: %s\n", pacOutputPrefix, iLoopCount, pstTeletextDescriptor->astTeletext_Info[iLoopCount].stISO_639_Language_code.aucPrivate_data_byte);
			printf("%s├─TeletextDescriptor.Teletext[%d].Teletext_type: 0x%02x\n", pacOutputPrefix, iLoopCount, pstTeletextDescriptor->astTeletext_Info[iLoopCount].uiTeletext_type);
			printf("%s├─TeletextDescriptor.Teletext[%d].Teletext_magazine_number: 0x%02x\n", pacOutputPrefix, iLoopCount, pstTeletextDescriptor->astTeletext_Info[iLoopCount].uiTeletext_magazine_number);
			printf("%s└─TeletextDescriptor.Teletext[%d].Teletext_page_number: 0x%02x\n", pacOutputPrefix, iLoopCount, pstTeletextDescriptor->astTeletext_Info[iLoopCount].uiTeletext_page_number);
		}
	}
	else
	{
		printf("%s└─TeletextDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstTeletextDescriptor->uiDescriptor_length);
	}

}

/*****************************************************************
 *
 * 打印Subtitling Descriptor描述子
 *
 *****************************************************************/
void Print_SubtitlingDescriptor(SUBTITLING_DESCRIPTOR_T *pstSubtitlingDescriptor, char *pacOutputPrefix)
{
	int iLoopCount = 0;
	printf("%sSubtitlingDescriptor:\n", pacOutputPrefix);
	printf("%s├─SubtitlingDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstSubtitlingDescriptor->uiDescriptor_tag);
	if (pstSubtitlingDescriptor->uiDescriptor_length > 0)
	{
		printf("%s├─SubtitlingDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstSubtitlingDescriptor->uiDescriptor_length);
		for (iLoopCount = 0; iLoopCount * 8 < pstSubtitlingDescriptor->uiDescriptor_length; iLoopCount++)
		{
			printf("%s├─SubtitlingDescriptor.Subtitling[%d].ISO_639_Language_code: %s\n", pacOutputPrefix, iLoopCount, pstSubtitlingDescriptor->astSubtitling_Info[iLoopCount].stISO_639_Language_code.aucPrivate_data_byte);
			printf("%s├─SubtitlingDescriptor.Subtitling[%d].Subtitling_type: 0x%02x\n", pacOutputPrefix, iLoopCount, pstSubtitlingDescriptor->astSubtitling_Info[iLoopCount].uiSubtitling_type);
			printf("%s├─SubtitlingDescriptor.Subtitling[%d].Composition_page_id: 0x%02x\n", pacOutputPrefix, iLoopCount, pstSubtitlingDescriptor->astSubtitling_Info[iLoopCount].uiComposition_page_id);
			printf("%s└─SubtitlingDescriptor.Subtitling[%d].Ancillary_page_id: 0x%02x\n", pacOutputPrefix, iLoopCount, pstSubtitlingDescriptor->astSubtitling_Info[iLoopCount].uiAncillary_page_id);
		}
	}
	else
	{
		printf("%s└─SubtitlingDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstSubtitlingDescriptor->uiDescriptor_length);
	}

}

/*****************************************************************
 *
 * 打印ShortEvent Descriptor描述子
 *
 *****************************************************************/
void Print_ShortEventDescriptor(SHORT_EVENT_DESCRIPTOR_T *pstShortEventDescriptor, char *pacOutputPrefix)
{
	printf("%sShortEventDescriptor:\n", pacOutputPrefix);
	printf("%s├─ShortEventDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstShortEventDescriptor->uiDescriptor_tag);
	
	if (pstShortEventDescriptor->uiDescriptor_length > 0)
	{
		printf("%s├─ShortEventDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstShortEventDescriptor->uiDescriptor_length);
		printf("%s├─ShortEventDescriptor.ISO_639_Language_code: %s\n", pacOutputPrefix, pstShortEventDescriptor->stISO_639_Language_code.aucPrivate_data_byte);
		printf("%s├─ShortEventDescriptor.Event_name_length: 0x%02x\n", pacOutputPrefix, pstShortEventDescriptor->uiEvent_name_length);
		if (pstShortEventDescriptor->uiEvent_name_length > 0)
		{
			printf("%s├─ShortEventDescriptor.Event_name_char: %s\n", pacOutputPrefix, pstShortEventDescriptor->aucEvent_name_char);
		}

		if (pstShortEventDescriptor->uiText_length > 0)
		{
			printf("%s├─ShortEventDescriptor.Text_length: 0x%02x\n", pacOutputPrefix, pstShortEventDescriptor->uiText_length);
			printf("%s└─ShortEventDescriptor.Text_char: %s\n", pacOutputPrefix, pstShortEventDescriptor->aucText_char);
		}
		else
		{
			printf("%s└─ShortEventDescriptor.Text_length: 0x%02x\n", pacOutputPrefix, pstShortEventDescriptor->uiText_length);
		}
	}
	else
	{
		printf("%s└─ShortEventDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstShortEventDescriptor->uiDescriptor_length);
	}
	
}

/*****************************************************************
 *
 * 打印Extended Event Descriptor描述子
 *
 *****************************************************************/
void Print_ExtendedEventDescriptor(EXTENDED_EVENT_DESCRIPTOR_T *pstExtendedEventDescriptor, char *pacOutputPrefix)
{
	int iLoopCount = 0;
	int iDescriptorInfoPostion = 0;
	
	printf("%sExtendedEventDescriptor:\n", pacOutputPrefix);
	printf("%s├─ExtendedEventDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstExtendedEventDescriptor->uiDescriptor_tag);
	
	if (pstExtendedEventDescriptor->uiDescriptor_length > 0)
	{
		printf("%s├─ExtendedEventDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstExtendedEventDescriptor->uiDescriptor_length);
		printf("%s├─ExtendedEventDescriptor.Descriptor_number: 0x%02x\n", pacOutputPrefix, pstExtendedEventDescriptor->uiDescriptor_number);
		printf("%s├─ExtendedEventDescriptor.Last_descriptor_number: 0x%02x\n", pacOutputPrefix, pstExtendedEventDescriptor->uiLast_descriptor_number);
		printf("%s├─ExtendedEventDescriptor.ISO_639_language_code: %s\n", pacOutputPrefix, pstExtendedEventDescriptor->stISO_639_language_code.aucPrivate_data_byte);
		printf("%s├─ExtendedEventDescriptor.Length_of_items: 0x%02x\n", pacOutputPrefix, pstExtendedEventDescriptor->uiLength_of_items);
		if (pstExtendedEventDescriptor->uiLength_of_items > 0)
		{
			for (iDescriptorInfoPostion = 0; iDescriptorInfoPostion < pstExtendedEventDescriptor->uiLength_of_items; iDescriptorInfoPostion += 2)
			{
				printf("%s├─ExtendedEventDescriptor.Extended_event_info[%d].Item_descriptor_length: 0x%02x\n", pacOutputPrefix, iLoopCount, pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].uiItem_descriptor_length);
				if (pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].uiItem_descriptor_length > 0)
				{
					printf("%s├─ExtendedEventDescriptor.Extended_event_info[%d].Item_descriptor_length: %s\n", pacOutputPrefix, iLoopCount, pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].aucItem_descriptor_char);
				}
				iDescriptorInfoPostion += pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].uiItem_descriptor_length;
				printf("%s├─ExtendedEventDescriptor.Extended_event_info[%d].Item_length: 0x%02x\n", pacOutputPrefix, iLoopCount, pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].uiItem_length);
				if (pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].uiItem_length > 0)
				{
					printf("%s├─ExtendedEventDescriptor.Extended_event_info[%d].Item_char: %s\n", pacOutputPrefix, iLoopCount, pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].aucItem_char);
				}
				iDescriptorInfoPostion += pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].uiItem_length;
			}
		}
		else
		{
		}
		if (pstExtendedEventDescriptor->uiText_length > 0)
		{
			printf("%s├─ExtendedEventDescriptor.Text_length: 0x%02x\n", pacOutputPrefix, pstExtendedEventDescriptor->uiText_length);
			printf("%s└─ExtendedEventDescriptor.Text_char: %s\n", pacOutputPrefix, pstExtendedEventDescriptor->aucText_char);
		}
		else
		{
			printf("%s└─ExtendedEventDescriptor.Text_length: 0x%02x\n", pacOutputPrefix, pstExtendedEventDescriptor->uiText_length);
		}
	}
	else
	{
		printf("%s└─ExtendedEventDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstExtendedEventDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印Frequency List Descriptor描述子
 *
 *****************************************************************/
void Print_FrequencyListDescriptor(FREQUENCY_LIST_DESCRIPTOR_T *pstFrequencyListDescriptor, char *pacOutputPrefix)
{
	int iLoopCount = 0;
	printf("%sFrequencyListDescriptor:\n", pacOutputPrefix);
	printf("%s├─FrequencyListDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstFrequencyListDescriptor->uiDescriptor_tag);
	
	if (pstFrequencyListDescriptor->uiDescriptor_length > 0)
	{
		printf("%s├─FrequencyListDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstFrequencyListDescriptor->uiDescriptor_length);
		printf("%s├─FrequencyListDescriptor.Reserved_future_use: 0x%02x\n", pacOutputPrefix, pstFrequencyListDescriptor->uiReserved_future_use);
		printf("%s├─FrequencyListDescriptor.Coding_type: 0x%02x\n", pacOutputPrefix, pstFrequencyListDescriptor->uiCoding_type);
		if (pstFrequencyListDescriptor->uiDescriptor_length - 1 > 0)
		{
			for (iLoopCount = 0; iLoopCount * 4 < pstFrequencyListDescriptor->uiDescriptor_length - 1; iLoopCount++)
			{
				printf("%s├─FrequencyListDescriptor.Centre_frequency[%d]: 0x%08x\n", pacOutputPrefix, iLoopCount, pstFrequencyListDescriptor->astCentre_frequency[iLoopCount].uiCentre_frequency);
			}
		}
	}
	else
	{
		printf("%s└─FrequencyListDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstFrequencyListDescriptor->uiDescriptor_length);
	}
	
}

