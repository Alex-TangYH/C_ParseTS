#include <stdio.h>
#include <string.h>
#include "Parse_Descriptor.h"
#include "Print_Descriptor.h"
#include "TsParser.h"
#include "FormatUtils.h"

/*****************************************************************
 *
 * 打印Network Name Descriptor描述子
 *
 *****************************************************************/
void Print_NetworkNameDescriptor(NETWORK_NAME_DESCRIPTOR_T *pstNetworkNameDescriptor, char *pacOutputPrefix)
{
	DUBUGPRINTF("%sNetworkNameDescriptor:\n", pacOutputPrefix);
	DUBUGPRINTF("%s├─NetworkNameDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstNetworkNameDescriptor->uiDescriptor_tag);
	if (pstNetworkNameDescriptor->uiDescriptor_length > 0)
	{
		DUBUGPRINTF("%s├─NetworkNameDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstNetworkNameDescriptor->uiDescriptor_length);
		DUBUGPRINTF("%s└─NetworkNameDescriptor.NewworkName: %s\n", pacOutputPrefix, pstNetworkNameDescriptor->aucInfo);
	}
	else
	{
		DUBUGPRINTF("%s└─NetworkNameDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstNetworkNameDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印Bouquet Name Descriptor描述子
 *
 *****************************************************************/

void Print_BouquetNameDescriptor(BOUQUET_NAME_DESCRIPTOR_T *pstBouquetNameDescriptor, char *pacOutputPrefix)
{
	DUBUGPRINTF("%sBouquetNameDescriptor:\n", pacOutputPrefix);
	DUBUGPRINTF("%s├─BouquetNameDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstBouquetNameDescriptor->uiDescriptor_tag);
	
	if (pstBouquetNameDescriptor->uiDescriptor_length > 0)
	{
		DUBUGPRINTF("%s├─BouquetNameDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstBouquetNameDescriptor->uiDescriptor_length);
		DUBUGPRINTF("%s└─BouquetNameDescriptor.Info: %s\n", pacOutputPrefix, pstBouquetNameDescriptor->aucInfo);
	}
	else
	{
		DUBUGPRINTF("%s└─BouquetNameDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstBouquetNameDescriptor->uiDescriptor_length);
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
	
	DUBUGPRINTF("%sServiceListDescriptor:\n", pacOutputPrefix);
	DUBUGPRINTF("%s├─ServiceListDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstServiceListDescriptor->uiDescriptor_tag);
	
	if (pstServiceListDescriptor->uiDescriptor_length > 0)
	{
		DUBUGPRINTF("%s├─ServiceListDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstServiceListDescriptor->uiDescriptor_length);
		for (iDescriptorInfoPosition = 0; iDescriptorInfoPosition < pstServiceListDescriptor->uiDescriptor_length; iDescriptorInfoPosition += 3)
		{
			
			if (pstServiceListDescriptor->uiDescriptor_length - iDescriptorInfoPosition > 2)
			{
				DUBUGPRINTF("%s├─ServiceListDescriptor.Service_info[%d].Serviec_id: 0x%02x\n", pacOutputPrefix, iDescriptorInfoCount, pstServiceListDescriptor->astService_info[iDescriptorInfoCount].uiServiec_id);
				DUBUGPRINTF("%s└─ServiceListDescriptor.Service_info[%d].Service_type: 0x%02x\n", pacOutputPrefix, iDescriptorInfoCount, pstServiceListDescriptor->astService_info[iDescriptorInfoCount].uiService_type);
			}
			else
			{
				DUBUGPRINTF("%s└─ServiceListDescriptor.Service_info[%d].Serviec_id: 0x%02x\n", pacOutputPrefix, iDescriptorInfoCount, pstServiceListDescriptor->astService_info[iDescriptorInfoCount].uiServiec_id);
			}
			iDescriptorInfoCount++;
		}
	}
	else
	{
		DUBUGPRINTF("%s└─ServiceListDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstServiceListDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印Satellite Delivery System Descriptor描述子
 *
 *****************************************************************/
void Print_SatelliteDeliverySystemDescriptor(SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR_T *pstSatelliteDeliverySystemDescriptor, char *pacOutputPrefix)
{
	DUBUGPRINTF("%sSatelliteDeliverySystemDescriptor:\n", pacOutputPrefix);
	DUBUGPRINTF("%s├─SatelliteDeliverySystemDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiDescriptor_tag);
	if (pstSatelliteDeliverySystemDescriptor->uiDescriptor_length > 0)
	{
		DUBUGPRINTF("%s├─SatelliteDeliverySystemDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiDescriptor_length);
		DUBUGPRINTF("%s├─SatelliteDeliverySystemDescriptor.Frequency: 0x%08x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiFrequency);
		DUBUGPRINTF("%s├─SatelliteDeliverySystemDescriptor.Orbital_position: 0x%04x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiOrbital_position);
		DUBUGPRINTF("%s├─SatelliteDeliverySystemDescriptor.West_east_flag: 0x%02x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiWest_east_flag);
		DUBUGPRINTF("%s├─SatelliteDeliverySystemDescriptor.Polarization: 0x%02x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiPolarization);
		DUBUGPRINTF("%s├─SatelliteDeliverySystemDescriptor.Roll_off: 0x%02x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiRoll_off);
		DUBUGPRINTF("%s├─SatelliteDeliverySystemDescriptor.Modulation_system: 0x%02x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiModulation_system);
		DUBUGPRINTF("%s├─SatelliteDeliverySystemDescriptor.Modulation_type: 0x%02x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiModulation_type);
		DUBUGPRINTF("%s├─SatelliteDeliverySystemDescriptor.Symbol_rate: 0x%02x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiSymbol_rate);
		DUBUGPRINTF("%s└─SatelliteDeliverySystemDescriptor.FEC_inner: 0x%02x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiFEC_inner);
	}
	else
	{
		DUBUGPRINTF("%s└─SatelliteDeliverySystemDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstSatelliteDeliverySystemDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印Terrestrial Delivery System Descriptor描述子
 *
 *****************************************************************/
void Print_TerrestrialDeliverySystemDescriptor(TERRESTRIAL_DELIVERY_SYSTEM_DESCRIPTOR_T *pstTerrestrialDeliverySystemDescriptor, char *pacOutputPrefix)
{
	DUBUGPRINTF("%sTerrestrialDeliverySystemDescriptor:\n", pacOutputPrefix);
	DUBUGPRINTF("%s├─TerrestrialDeliverySystemDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiDescriptor_tag);
	if (pstTerrestrialDeliverySystemDescriptor->uiDescriptor_length > 0)
	{
		DUBUGPRINTF("%s├─TerrestrialDeliverySystemDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiDescriptor_length);
		DUBUGPRINTF("%s├─TerrestrialDeliverySystemDescriptor.Centre_frequency: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiCentre_frequency);
		DUBUGPRINTF("%s├─TerrestrialDeliverySystemDescriptor.Bandwidth: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiBandwidth);
		DUBUGPRINTF("%s├─TerrestrialDeliverySystemDescriptor.Priority: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiPriority);
		DUBUGPRINTF("%s├─TerrestrialDeliverySystemDescriptor.Time_Slicing_indicator: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiTime_Slicing_indicator);
		DUBUGPRINTF("%s├─TerrestrialDeliverySystemDescriptor.MPE_FEC_indicator: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiMPE_FEC_indicator);
		DUBUGPRINTF("%s├─TerrestrialDeliverySystemDescriptor.Reserved_future_use_first: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiReserved_future_use_first);
		DUBUGPRINTF("%s├─TerrestrialDeliverySystemDescriptor.Constellation: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiConstellation);
		DUBUGPRINTF("%s├─TerrestrialDeliverySystemDescriptor.Hierarchy_information: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiHierarchy_information);
		DUBUGPRINTF("%s├─TerrestrialDeliverySystemDescriptor.Code_rate_HP_stream: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiCode_rate_HP_stream);
		DUBUGPRINTF("%s├─TerrestrialDeliverySystemDescriptor.Code_rate_LP_stream: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiCode_rate_LP_stream);
		DUBUGPRINTF("%s├─TerrestrialDeliverySystemDescriptor.Guard_interval: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiGuard_interval);
		DUBUGPRINTF("%s├─TerrestrialDeliverySystemDescriptor.Transmission_mode: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiTransmission_mode);
		DUBUGPRINTF("%s├─TerrestrialDeliverySystemDescriptor.Other_frequency_flag: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiOther_frequency_flag);
		DUBUGPRINTF("%s└─TerrestrialDeliverySystemDescriptor.uiReserved_future_use_second: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiReserved_future_use_second);
	}
	else
	{
		DUBUGPRINTF("%s└─TerrestrialDeliverySystemDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstTerrestrialDeliverySystemDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印Cable Delivery System Descriptor描述子
 *
 *****************************************************************/
void Print_CableDeliverySystemDescriptor(CABLE_DELIVERY_SYSTEM_DESCRIPTOR_T *pstCableDeliverySystemDescriptor, char *pacOutputPrefix)
{
	DUBUGPRINTF("%sCableDeliverySystemDescriptor:\n", pacOutputPrefix);
	DUBUGPRINTF("%s├─CableDeliverySystemDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstCableDeliverySystemDescriptor->uiDescriptor_tag);
	if (pstCableDeliverySystemDescriptor->uiDescriptor_length > 0)
	{
		DUBUGPRINTF("%s├─CableDeliverySystemDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstCableDeliverySystemDescriptor->uiDescriptor_length);
		DUBUGPRINTF("%s├─CableDeliverySystemDescriptor.Frequency: 0x%02x\n", pacOutputPrefix, pstCableDeliverySystemDescriptor->uiFrequency);
		DUBUGPRINTF("%s├─CableDeliverySystemDescriptor.Reserved_future_use: 0x%02x\n", pacOutputPrefix, pstCableDeliverySystemDescriptor->uiReserved_future_use);
		DUBUGPRINTF("%s├─CableDeliverySystemDescriptor.Fec_outer: 0x%02x\n", pacOutputPrefix, pstCableDeliverySystemDescriptor->uiFec_outer);
		DUBUGPRINTF("%s├─CableDeliverySystemDescriptor.Modulation: 0x%02x\n", pacOutputPrefix, pstCableDeliverySystemDescriptor->uiModulation);
		DUBUGPRINTF("%s├─CableDeliverySystemDescriptor.Symbol_rate: 0x%02x\n", pacOutputPrefix, pstCableDeliverySystemDescriptor->uiSymbol_rate);
		DUBUGPRINTF("%s└─CableDeliverySystemDescriptor.FEC_inner: 0x%02x\n", pacOutputPrefix, pstCableDeliverySystemDescriptor->uiFEC_inner);
	}
	else
	{
		DUBUGPRINTF("%s└─CableDeliverySystemDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstCableDeliverySystemDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印ServiceDescriptor描述子
 *
 *****************************************************************/
void Print_ServiceDescriptor(SERVICE_DESCRIPTOR_T *pstServiceDescriptor, char *pacOutputPrefix)
{
	DUBUGPRINTF("%sServiceDescriptor:\n", pacOutputPrefix);
	DUBUGPRINTF("%s├─ServiceDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstServiceDescriptor->uiDescriptor_tag);
	if (pstServiceDescriptor->uiDescriptor_length > 0)
	{
		DUBUGPRINTF("%s├─ServiceDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstServiceDescriptor->uiDescriptor_length);
		DUBUGPRINTF("%s├─ServiceDescriptor.Service_type: 0x%02x\n", pacOutputPrefix, pstServiceDescriptor->uiService_type);
		DUBUGPRINTF("%s├─ServiceDescriptor.Service_provider_name_length: 0x%02x\n", pacOutputPrefix, pstServiceDescriptor->uiService_provider_name_length);
		if (pstServiceDescriptor->uiService_provider_name_length > 0)
		{
			DUBUGPRINTF("%s├─ServiceDescriptor.Service_provider_name: %s\n", pacOutputPrefix, pstServiceDescriptor->aucService_provider_name);
		}
		else
		{
		}
		
		if (pstServiceDescriptor->uiService_provider_name_length > 0)
		{
			DUBUGPRINTF("%s├─ServiceDescriptor.Service_name_length: 0x%02x\n", pacOutputPrefix, pstServiceDescriptor->uiService_name_length);
			DUBUGPRINTF("%s└─ServiceDescriptor.Service_name: %s\n", pacOutputPrefix, pstServiceDescriptor->aucService_name);
		}
		else
		{
			DUBUGPRINTF("%s└─ServiceDescriptor.Service_name_length: 0x%02x\n", pacOutputPrefix, pstServiceDescriptor->uiService_name_length);
		}
	}
	else
	{
		DUBUGPRINTF("%s└─ServiceDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstServiceDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印LinkageDescriptor描述子
 *
 *****************************************************************/
void Print_LinkageDescriptor(LINKAGE_DESCRIPTOR_T *pstLinkageDescriptor, char *pacOutputPrefix)
{
	DUBUGPRINTF("%sLinkageDescriptor:\n", pacOutputPrefix);
	DUBUGPRINTF("%s├─LinkageDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->uiDescriptor_tag);
	if (pstLinkageDescriptor->uiDescriptor_length > 0)
	{
		DUBUGPRINTF("%s├─LinkageDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->uiDescriptor_length);
		DUBUGPRINTF("%s├─LinkageDescriptor.Transport_stream_id: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->uiTransport_stream_id);
		DUBUGPRINTF("%s├─LinkageDescriptor.Original_network_id: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->uiOriginal_network_id);
		DUBUGPRINTF("%s├─LinkageDescriptor.Service_id: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->uiService_id);
		DUBUGPRINTF("%s├─LinkageDescriptor.Linage_type: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->uiLinage_type);
		if (0x08 == pstLinkageDescriptor->uiLinage_type)
		{
			DUBUGPRINTF("%s├─LinkageDescriptor.Hand_over_type: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiHand_over_type);
			DUBUGPRINTF("%s├─LinkageDescriptor.Reserved_future_use: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiReserved_future_use);
			DUBUGPRINTF("%s├─LinkageDescriptor.Origin_type: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiOrigin_type);
			if ((1 == pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiHand_over_type) || (2 == pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiHand_over_type)
					|| (3 == pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiHand_over_type))
			{
				DUBUGPRINTF("%s├─LinkageDescriptor.Network_id: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiNetwork_id);
			}
			if (0 == pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiOrigin_type)
			{
				DUBUGPRINTF("%s├─LinkageDescriptor.Initial_service_id: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_08.uiInitial_service_id);
			}
		}
		else if (0x0D == pstLinkageDescriptor->uiLinage_type)
		{
			DUBUGPRINTF("%s├─LinkageDescriptor.Target_event_id: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_0d.uiTarget_event_id);
			DUBUGPRINTF("%s├─LinkageDescriptor.Target_listed: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_0d.uiTarget_listed);
			DUBUGPRINTF("%s├─LinkageDescriptor.Event_simulcast: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_0d.uiEvent_simulcast);
			DUBUGPRINTF("%s├─LinkageDescriptor.Reserved: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->stLinkage_info_union.stLinkage_union_0d.uiReserved);
		}
		else
		{
		}
		DUBUGPRINTF("%s└─LinkageDescriptor.Private_data: %s\n", pacOutputPrefix, pstLinkageDescriptor->aucPrivate_data);
	}
	else
	{
		DUBUGPRINTF("%s└─LinkageDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstLinkageDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印Stream Indentifier Descriptor描述子
 *
 *****************************************************************/
void Print_StreamIndentifierDescriptor(STREAM_IDENTIFIER_DESCRIPTOR_T *pstStreamIndentifierDescriptor, char *pacOutputPrefix)
{
	DUBUGPRINTF("%sStreamIndentifierDescriptor:\n", pacOutputPrefix);
	DUBUGPRINTF("%s├─StreamIndentifierDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstStreamIndentifierDescriptor->uiDescriptor_tag);
	if (pstStreamIndentifierDescriptor->uiDescriptor_length > 0)
	{
		DUBUGPRINTF("%s├─StreamIndentifierDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstStreamIndentifierDescriptor->uiDescriptor_length);
		DUBUGPRINTF("%s└─StreamIndentifierDescriptor.Component_tag: 0x%02x\n", pacOutputPrefix, pstStreamIndentifierDescriptor->uiComponent_tag);
	}
	else
	{
		DUBUGPRINTF("%s└─StreamIndentifierDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstStreamIndentifierDescriptor->uiDescriptor_length);
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
	DUBUGPRINTF("%sVideoStreamDescriptor:\n", pacOutputPrefix);
	DUBUGPRINTF("%s├─VideoStreamDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiDescriptor_tag);
	if (pstVideoStreamDescriptor->uiDescriptor_length > 0)
	{
		DUBUGPRINTF("%s├─VideoStreamDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiDescriptor_length);
		DUBUGPRINTF("%s├─VideoStreamDescriptor.uiMultiple_frame_rate_flag: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiMultiple_frame_rate_flag);
		DUBUGPRINTF("%s├─VideoStreamDescriptor.uiFrame_rate_code: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiFrame_rate_code);
		DUBUGPRINTF("%s├─VideoStreamDescriptor.uiMPEG_2_flag: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiMPEG_2_flag);
		DUBUGPRINTF("%s├─VideoStreamDescriptor.uiConstrained_parameter_flag: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiConstrained_parameter_flag);
		
		if (1 == pstVideoStreamDescriptor->uiMPEG_2_flag)
		{
			DUBUGPRINTF("%s├─VideoStreamDescriptor.uiStill_picture_flag: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiStill_picture_flag);
			DUBUGPRINTF("%s├─VideoStreamDescriptor.uiProfile_and_level_indication: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiProfile_and_level_indication);
			DUBUGPRINTF("%s├─VideoStreamDescriptor.uiChroma_format: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiChroma_format);
			DUBUGPRINTF("%s├─VideoStreamDescriptor.uiFrame_rate_extension_flag: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiFrame_rate_extension_flag);
			DUBUGPRINTF("%s└─VideoStreamDescriptor.uiReserved: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiReserved);
		}
		else
		{
			DUBUGPRINTF("%s└─VideoStreamDescriptor.uiStill_picture_flag: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiStill_picture_flag);
		}
		
	}
	else
	{
		DUBUGPRINTF("%s└─VideoStreamDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstVideoStreamDescriptor->uiDescriptor_length);
	}

}

/*****************************************************************
 *
 * 打印Audio Stream Descriptor描述子
 *
 *****************************************************************/
void Print_AudioStreamDescriptor(AUDIO_STREAM_DESCRIPTOR_T *pstAudioStreamDescriptor, char *pacOutputPrefix)
{
	DUBUGPRINTF("%sAudioStreamDescriptor:\n", pacOutputPrefix);
	DUBUGPRINTF("%s├─AudioStreamDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstAudioStreamDescriptor->uiDescriptor_tag);
	if (pstAudioStreamDescriptor->uiDescriptor_length > 0)
	{
		DUBUGPRINTF("%s├─AudioStreamDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstAudioStreamDescriptor->uiDescriptor_length);
		DUBUGPRINTF("%s├─AudioStreamDescriptor.Free_format_flag: 0x%02x\n", pacOutputPrefix, pstAudioStreamDescriptor->uiFree_format_flag);
		DUBUGPRINTF("%s├─AudioStreamDescriptor.ID: 0x%02x\n", pacOutputPrefix, pstAudioStreamDescriptor->uiID);
		DUBUGPRINTF("%s├─AudioStreamDescriptor.Layer: 0x%02x\n", pacOutputPrefix, pstAudioStreamDescriptor->uiLayer);
		DUBUGPRINTF("%s└─AudioStreamDescriptor.Reserved: 0x%02x\n", pacOutputPrefix, pstAudioStreamDescriptor->uiReserved);
	}
	else
	{
		DUBUGPRINTF("%s└─AudioStreamDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstAudioStreamDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印DataStreamAlignmentDescriptor描述子
 *
 *****************************************************************/
void Print_DataStreamAlignmentDescriptor(DATA_STREAM_ALIGNMENT_DESCRIPTOR_T *pstDataStreamAlignmentDescriptor, char *pacOutputPrefix)
{
	DUBUGPRINTF("%sDataStreamAlignmentDescriptor:\n", pacOutputPrefix);
	DUBUGPRINTF("%s├─DataStreamAlignmentDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstDataStreamAlignmentDescriptor->uiDescriptor_tag);
	if (pstDataStreamAlignmentDescriptor->uiDescriptor_length > 0)
	{
		DUBUGPRINTF("%s├─DataStreamAlignmentDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstDataStreamAlignmentDescriptor->uiDescriptor_length);
		DUBUGPRINTF("%s└─DataStreamAlignmentDescriptor.Alignment_type: 0x%02x\n", pacOutputPrefix, pstDataStreamAlignmentDescriptor->uiAlignment_type);
	}
	else
	{
		DUBUGPRINTF("%s└─DataStreamAlignmentDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstDataStreamAlignmentDescriptor->uiDescriptor_length);
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
	
	DUBUGPRINTF("%sISO_639_LanguageDescriptor:\n", pacOutputPrefix);
	DUBUGPRINTF("%s├─ISO_639_LanguageDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstISO_639_LanguageDescriptor->uiDescriptor_tag);
	if (pstISO_639_LanguageDescriptor->uiDescriptor_length > 0)
	{
		DUBUGPRINTF("%s├─ISO_639_LanguageDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstISO_639_LanguageDescriptor->uiDescriptor_length);
		
		for (iDescriptorLength = 0; iDescriptorLength < pstISO_639_LanguageDescriptor->uiDescriptor_length - 1; iDescriptorLength += 3)
		{
			DUBUGPRINTF("%s├─ISO_639_LanguageDescriptor.Descriptor_length: %s\n", pacOutputPrefix, pstISO_639_LanguageDescriptor->stISO_639_Language_code[iDescriptorLoopCount].aucPrivate_data_byte);
			iDescriptorLoopCount++;
		}
		DUBUGPRINTF("%s└─ISO_639_LanguageDescriptor.Audio_type: 0x%02x\n", pacOutputPrefix, pstISO_639_LanguageDescriptor->uiAudio_type);
	}
	else
	{
		DUBUGPRINTF("%s└─ISO_639_LanguageDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstISO_639_LanguageDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印Maximum Bitrate Descriptor描述子
 *
 *****************************************************************/
void Print_MaximumBitrateDescriptor(MAXIMUM_BITRATE_DESCRIPTOR_T *pstMaximumBitrateDescriptor, char *pacOutputPrefix)
{
	DUBUGPRINTF("%sMaximumBitrateDescriptor:\n", pacOutputPrefix);
	DUBUGPRINTF("%s├─MaximumBitrateDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstMaximumBitrateDescriptor->uiDescriptor_tag);
	if (pstMaximumBitrateDescriptor->uiDescriptor_length > 0)
	{
		DUBUGPRINTF("%s├─MaximumBitrateDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstMaximumBitrateDescriptor->uiDescriptor_length);
		DUBUGPRINTF("%s├─MaximumBitrateDescriptor.Reserved: 0x%02x\n", pacOutputPrefix, pstMaximumBitrateDescriptor->uiReserved);
		DUBUGPRINTF("%s└─MaximumBitrateDescriptor.Maximum_bitrate: 0x%02x\n", pacOutputPrefix, pstMaximumBitrateDescriptor->uiMaximum_bitrate);
	}
	else
	{
		DUBUGPRINTF("%s└─MaximumBitrateDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstMaximumBitrateDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印CA_Descriptor描述子
 *
 *****************************************************************/
void Print_CA_Descriptor(CA_DESCRIPTOR_T *pstCA_Descriptor, char *pacOutputPrefix)
{
	DUBUGPRINTF("%sCA_Descriptor:\n", pacOutputPrefix);
	DUBUGPRINTF("%s├─CA_Descriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstCA_Descriptor->uiDescriptor_tag);
	if (pstCA_Descriptor->uiDescriptor_length > 0)
	{
		DUBUGPRINTF("%s├─CA_Descriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstCA_Descriptor->uiDescriptor_length);
		DUBUGPRINTF("%s├─CA_Descriptor.CA_system_ID: 0x%02x\n", pacOutputPrefix, pstCA_Descriptor->uiCA_system_ID);
		DUBUGPRINTF("%s├─CA_Descriptor.Reserved: 0x%02x\n", pacOutputPrefix, pstCA_Descriptor->uiReserved);
		if (pstCA_Descriptor->uiDescriptor_length - 4 > 0)
		{
			DUBUGPRINTF("%s├─CA_Descriptor.CA_PID: 0x%02x\n", pacOutputPrefix, pstCA_Descriptor->uiCA_PID);
			DUBUGPRINTF("%s└─CA_Descriptor.Private_data_byte: %s\n", pacOutputPrefix, pstCA_Descriptor->aucPrivate_data_byte);
		}
		else
		{
			DUBUGPRINTF("%s└─CA_Descriptor.CA_PID: 0x%02x\n", pacOutputPrefix, pstCA_Descriptor->uiCA_PID);
		}
		
	}
	else
	{
		DUBUGPRINTF("%s└─CA_Descriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstCA_Descriptor->uiDescriptor_length);
	}

}

/*****************************************************************
 *
 * 打印System Clock Descriptor描述子
 *
 *****************************************************************/
void Print_SystemClockDescriptor(SYSTEM_CLOCK_DESCRIPTOR_T *pstSystemClockDescriptor, char *pacOutputPrefix)
{
	DUBUGPRINTF("%sSystemClockDescriptor:\n", pacOutputPrefix);
	DUBUGPRINTF("%s├─SystemClockDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstSystemClockDescriptor->uiDescriptor_tag);
	if (pstSystemClockDescriptor->uiDescriptor_length > 0)
	{
		DUBUGPRINTF("%s├─SystemClockDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstSystemClockDescriptor->uiDescriptor_length);
		DUBUGPRINTF("%s├─SystemClockDescriptor.External_clock_reference_indicator: 0x%02x\n", pacOutputPrefix, pstSystemClockDescriptor->uiExternal_clock_reference_indicator);
		DUBUGPRINTF("%s├─SystemClockDescriptor.Reserved_first: 0x%02x\n", pacOutputPrefix, pstSystemClockDescriptor->uiReserved_first);
		DUBUGPRINTF("%s├─SystemClockDescriptor.Clock_accuracy_integer: 0x%02x\n", pacOutputPrefix, pstSystemClockDescriptor->uiClock_accuracy_integer);
		DUBUGPRINTF("%s├─SystemClockDescriptor.Clock_accuracy_exponent: 0x%02x\n", pacOutputPrefix, pstSystemClockDescriptor->uiClock_accuracy_exponent);
		DUBUGPRINTF("%s└─SystemClockDescriptor.Reserved_second: 0x%02x\n", pacOutputPrefix, pstSystemClockDescriptor->uiReserved_second);
	}
	else
	{
		DUBUGPRINTF("%s└─SystemClockDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstSystemClockDescriptor->uiDescriptor_length);
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
	DUBUGPRINTF("%sTeletextDescriptor:\n", pacOutputPrefix);
	DUBUGPRINTF("%s├─TeletextDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstTeletextDescriptor->uiDescriptor_tag);
	if (pstTeletextDescriptor->uiDescriptor_length > 0)
	{
		DUBUGPRINTF("%s├─TeletextDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstTeletextDescriptor->uiDescriptor_length);
		for (iLoopCount = 0; iLoopCount * 5 < pstTeletextDescriptor->uiDescriptor_length; iLoopCount++)
		{
			DUBUGPRINTF("%s├─TeletextDescriptor.Teletext[%d].ISO_639_Language_code: %s\n", pacOutputPrefix, iLoopCount, pstTeletextDescriptor->astTeletext_Info[iLoopCount].stISO_639_Language_code.aucPrivate_data_byte);
			DUBUGPRINTF("%s├─TeletextDescriptor.Teletext[%d].Teletext_type: 0x%02x\n", pacOutputPrefix, iLoopCount, pstTeletextDescriptor->astTeletext_Info[iLoopCount].uiTeletext_type);
			DUBUGPRINTF("%s├─TeletextDescriptor.Teletext[%d].Teletext_magazine_number: 0x%02x\n", pacOutputPrefix, iLoopCount, pstTeletextDescriptor->astTeletext_Info[iLoopCount].uiTeletext_magazine_number);
			DUBUGPRINTF("%s└─TeletextDescriptor.Teletext[%d].Teletext_page_number: 0x%02x\n", pacOutputPrefix, iLoopCount, pstTeletextDescriptor->astTeletext_Info[iLoopCount].uiTeletext_page_number);
		}
	}
	else
	{
		DUBUGPRINTF("%s└─TeletextDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstTeletextDescriptor->uiDescriptor_length);
	}

}

/*****************************************************************
 *
 * 打印Local Time Offset Descriptor描述子
 *
 *****************************************************************/
void Print_LocalTimeOffsetDescriptor(LOCAL_TIME_OFFSET_DESCRIPTOR_T *pstLocalTimeOffsetDescriptor, char *pacOutputPrefix)
{
	int iLoopCount = 0;
	int iOneInfoLength = 3 + 1 + 2 + 5 + 2;
	DUBUGPRINTF("%sLocalTimeOffsetDescriptor:\n", pacOutputPrefix);
	DUBUGPRINTF("%s├─LocalTimeOffsetDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstLocalTimeOffsetDescriptor->uiDescriptor_tag);
	if (pstLocalTimeOffsetDescriptor->uiDescriptor_length > 0)
	{
		DUBUGPRINTF("%s├─LocalTimeOffsetDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstLocalTimeOffsetDescriptor->uiDescriptor_length);
		for (iLoopCount = 0; iLoopCount * iOneInfoLength < pstLocalTimeOffsetDescriptor->uiDescriptor_length; iLoopCount++)
		{
			DUBUGPRINTF("%s├─LocalTimeOffsetDescriptor.Subtitling[%d].Country_code: %s\n", pacOutputPrefix, iLoopCount, pstLocalTimeOffsetDescriptor->astLocalTimeOffset_Info[iLoopCount].uiCountry_code);
			DUBUGPRINTF("%s├─LocalTimeOffsetDescriptor.Subtitling[%d].uiCountry_region_id: 0x%02x\n", pacOutputPrefix, iLoopCount, pstLocalTimeOffsetDescriptor->astLocalTimeOffset_Info[iLoopCount].uiCountry_region_id);
			DUBUGPRINTF("%s├─LocalTimeOffsetDescriptor.Subtitling[%d].uiReserved: 0x%02x\n", pacOutputPrefix, iLoopCount, pstLocalTimeOffsetDescriptor->astLocalTimeOffset_Info[iLoopCount].uiReserved);
			DUBUGPRINTF("%s├─LocalTimeOffsetDescriptor.Subtitling[%d].uiLocal_time_offset_polarity: 0x%02x\n", pacOutputPrefix, iLoopCount, pstLocalTimeOffsetDescriptor->astLocalTimeOffset_Info[iLoopCount].uiLocal_time_offset_polarity);
			DUBUGPRINTF("%s├─LocalTimeOffsetDescriptor.Subtitling[%d].uiLocal_time_offset: 0x%02x\n", pacOutputPrefix, iLoopCount, pstLocalTimeOffsetDescriptor->astLocalTimeOffset_Info[iLoopCount].uiLocal_time_offset);
			char acUTC_time[50] = { 0 };
			FormatUTC_TimeFormMJD(acUTC_time, pstLocalTimeOffsetDescriptor->astLocalTimeOffset_Info[iLoopCount].uiTime_of_change);
			DUBUGPRINTF("%s├─LocalTimeOffsetDescriptor.Subtitling[%d].uiTime_of_change: %s\n", pacOutputPrefix, iLoopCount, acUTC_time);
			DUBUGPRINTF("%s└─LocalTimeOffsetDescriptor.Subtitling[%d].uiNext_time_offset: 0x%02x\n", pacOutputPrefix, iLoopCount, pstLocalTimeOffsetDescriptor->astLocalTimeOffset_Info[iLoopCount].uiNext_time_offset);
		}
	}
	else
	{
		DUBUGPRINTF("%s└─LocalTimeOffsetDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstLocalTimeOffsetDescriptor->uiDescriptor_length);
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
	DUBUGPRINTF("%sSubtitlingDescriptor:\n", pacOutputPrefix);
	DUBUGPRINTF("%s├─SubtitlingDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstSubtitlingDescriptor->uiDescriptor_tag);
	if (pstSubtitlingDescriptor->uiDescriptor_length > 0)
	{
		DUBUGPRINTF("%s├─SubtitlingDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstSubtitlingDescriptor->uiDescriptor_length);
		for (iLoopCount = 0; iLoopCount * 8 < pstSubtitlingDescriptor->uiDescriptor_length; iLoopCount++)
		{
			DUBUGPRINTF("%s├─SubtitlingDescriptor.Subtitling[%d].ISO_639_Language_code: %s\n", pacOutputPrefix, iLoopCount, pstSubtitlingDescriptor->astSubtitling_Info[iLoopCount].stISO_639_Language_code.aucPrivate_data_byte);
			DUBUGPRINTF("%s├─SubtitlingDescriptor.Subtitling[%d].Subtitling_type: 0x%02x\n", pacOutputPrefix, iLoopCount, pstSubtitlingDescriptor->astSubtitling_Info[iLoopCount].uiSubtitling_type);
			DUBUGPRINTF("%s├─SubtitlingDescriptor.Subtitling[%d].Composition_page_id: 0x%02x\n", pacOutputPrefix, iLoopCount, pstSubtitlingDescriptor->astSubtitling_Info[iLoopCount].uiComposition_page_id);
			DUBUGPRINTF("%s└─SubtitlingDescriptor.Subtitling[%d].Ancillary_page_id: 0x%02x\n", pacOutputPrefix, iLoopCount, pstSubtitlingDescriptor->astSubtitling_Info[iLoopCount].uiAncillary_page_id);
		}
	}
	else
	{
		DUBUGPRINTF("%s└─SubtitlingDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstSubtitlingDescriptor->uiDescriptor_length);
	}
}

/*****************************************************************
 *
 * 打印ShortEvent Descriptor描述子
 *
 *****************************************************************/
void Print_ShortEventDescriptor(SHORT_EVENT_DESCRIPTOR_T *pstShortEventDescriptor, char *pacOutputPrefix)
{
	DUBUGPRINTF("%sShortEventDescriptor:\n", pacOutputPrefix);
	DUBUGPRINTF("%s├─ShortEventDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstShortEventDescriptor->uiDescriptor_tag);
	
	if (pstShortEventDescriptor->uiDescriptor_length > 0)
	{
		DUBUGPRINTF("%s├─ShortEventDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstShortEventDescriptor->uiDescriptor_length);
		DUBUGPRINTF("%s├─ShortEventDescriptor.ISO_639_Language_code: %s\n", pacOutputPrefix, pstShortEventDescriptor->stISO_639_Language_code.aucPrivate_data_byte);
		DUBUGPRINTF("%s├─ShortEventDescriptor.Event_name_length: 0x%02x\n", pacOutputPrefix, pstShortEventDescriptor->uiEvent_name_length);
		if (pstShortEventDescriptor->uiEvent_name_length > 0)
		{
			DUBUGPRINTF("%s├─ShortEventDescriptor.Event_name_char: %s\n", pacOutputPrefix, pstShortEventDescriptor->aucEvent_name_char);
		}

		if (pstShortEventDescriptor->uiText_length > 0)
		{
			DUBUGPRINTF("%s├─ShortEventDescriptor.Text_length: 0x%02x\n", pacOutputPrefix, pstShortEventDescriptor->uiText_length);
			DUBUGPRINTF("%s└─ShortEventDescriptor.Text_char: %s\n", pacOutputPrefix, pstShortEventDescriptor->aucText_char);
		}
		else
		{
			DUBUGPRINTF("%s└─ShortEventDescriptor.Text_length: 0x%02x\n", pacOutputPrefix, pstShortEventDescriptor->uiText_length);
		}
	}
	else
	{
		DUBUGPRINTF("%s└─ShortEventDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstShortEventDescriptor->uiDescriptor_length);
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
	
	DUBUGPRINTF("%sExtendedEventDescriptor:\n", pacOutputPrefix);
	DUBUGPRINTF("%s├─ExtendedEventDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstExtendedEventDescriptor->uiDescriptor_tag);
	
	if (pstExtendedEventDescriptor->uiDescriptor_length > 0)
	{
		DUBUGPRINTF("%s├─ExtendedEventDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstExtendedEventDescriptor->uiDescriptor_length);
		DUBUGPRINTF("%s├─ExtendedEventDescriptor.Descriptor_number: 0x%02x\n", pacOutputPrefix, pstExtendedEventDescriptor->uiDescriptor_number);
		DUBUGPRINTF("%s├─ExtendedEventDescriptor.Last_descriptor_number: 0x%02x\n", pacOutputPrefix, pstExtendedEventDescriptor->uiLast_descriptor_number);
		DUBUGPRINTF("%s├─ExtendedEventDescriptor.ISO_639_language_code: %s\n", pacOutputPrefix, pstExtendedEventDescriptor->stISO_639_language_code.aucPrivate_data_byte);
		DUBUGPRINTF("%s├─ExtendedEventDescriptor.Length_of_items: 0x%02x\n", pacOutputPrefix, pstExtendedEventDescriptor->uiLength_of_items);
		if (pstExtendedEventDescriptor->uiLength_of_items > 0)
		{
			for (iDescriptorInfoPostion = 0; iDescriptorInfoPostion < pstExtendedEventDescriptor->uiLength_of_items; iDescriptorInfoPostion += 2)
			{
				DUBUGPRINTF("%s├─ExtendedEventDescriptor.Extended_event_info[%d].Item_descriptor_length: 0x%02x\n", pacOutputPrefix, iLoopCount, pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].uiItem_descriptor_length);
				if (pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].uiItem_descriptor_length > 0)
				{
					DUBUGPRINTF("%s├─ExtendedEventDescriptor.Extended_event_info[%d].Item_descriptor_length: %s\n", pacOutputPrefix, iLoopCount, pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].aucItem_descriptor_char);
				}
				iDescriptorInfoPostion += pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].uiItem_descriptor_length;
				DUBUGPRINTF("%s├─ExtendedEventDescriptor.Extended_event_info[%d].Item_length: 0x%02x\n", pacOutputPrefix, iLoopCount, pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].uiItem_length);
				if (pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].uiItem_length > 0)
				{
					DUBUGPRINTF("%s├─ExtendedEventDescriptor.Extended_event_info[%d].Item_char: %s\n", pacOutputPrefix, iLoopCount, pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].aucItem_char);
				}
				iDescriptorInfoPostion += pstExtendedEventDescriptor->astExtended_event_info[iLoopCount].uiItem_length;
			}
		}
		else
		{
		}
		if (pstExtendedEventDescriptor->uiText_length > 0)
		{
			DUBUGPRINTF("%s├─ExtendedEventDescriptor.Text_length: 0x%02x\n", pacOutputPrefix, pstExtendedEventDescriptor->uiText_length);
			DUBUGPRINTF("%s└─ExtendedEventDescriptor.Text_char: %s\n", pacOutputPrefix, pstExtendedEventDescriptor->aucText_char);
		}
		else
		{
			DUBUGPRINTF("%s└─ExtendedEventDescriptor.Text_length: 0x%02x\n", pacOutputPrefix, pstExtendedEventDescriptor->uiText_length);
		}
	}
	else
	{
		DUBUGPRINTF("%s└─ExtendedEventDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstExtendedEventDescriptor->uiDescriptor_length);
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
	DUBUGPRINTF("%sFrequencyListDescriptor:\n", pacOutputPrefix);
	DUBUGPRINTF("%s├─FrequencyListDescriptor.Descriptor_tag: 0x%02x\n", pacOutputPrefix, pstFrequencyListDescriptor->uiDescriptor_tag);
	
	if (pstFrequencyListDescriptor->uiDescriptor_length > 0)
	{
		DUBUGPRINTF("%s├─FrequencyListDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstFrequencyListDescriptor->uiDescriptor_length);
		DUBUGPRINTF("%s├─FrequencyListDescriptor.Reserved_future_use: 0x%02x\n", pacOutputPrefix, pstFrequencyListDescriptor->uiReserved_future_use);
		DUBUGPRINTF("%s├─FrequencyListDescriptor.Coding_type: 0x%02x\n", pacOutputPrefix, pstFrequencyListDescriptor->uiCoding_type);
		if (pstFrequencyListDescriptor->uiDescriptor_length - 1 > 0)
		{
			for (iLoopCount = 0; iLoopCount * 4 < pstFrequencyListDescriptor->uiDescriptor_length - 1; iLoopCount++)
			{
				DUBUGPRINTF("%s├─FrequencyListDescriptor.Centre_frequency[%d]: 0x%08x\n", pacOutputPrefix, iLoopCount, pstFrequencyListDescriptor->astCentre_frequency[iLoopCount].uiCentre_frequency);
			}
		}
	}
	else
	{
		DUBUGPRINTF("%s└─FrequencyListDescriptor.Descriptor_length: 0x%02x\n", pacOutputPrefix, pstFrequencyListDescriptor->uiDescriptor_length);
	}
	
}

