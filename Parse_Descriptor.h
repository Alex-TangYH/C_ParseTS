#ifndef _Parse_Descriptor_
#define _Parse_Descriptor_

#define NETWORK_NAME_DESCRIPTOR_INFO_MAX 256
#define SERVICE_LIST_DESCRIPTOR_INFO_MAX 86
#define BOUQUET_NAME_DESCRIPTOR_INFO_MAX 256
#define SERVICE_DESCRIPTOR_INFO_MAX 256
#define CA_DESCRIPTOR_INFO_MAX 256 - 4
#define LINKAGE_DESCRIPTOR_INFO_MAX 256
#define ISO_639_LANGUAGE_CODE_MAX 85
#define TELETEXT_INFO_MAX 86
#define SUBTITLING_INFO_MAX 32
#define SHORT_EVENT_INFO_MAX 256
#define EXTENDED_EVENT_DESCRIPTOR_INFO_MAX 128
#define EXTENDED_EVENT_DESCRIPTOR_CHAR_MAX 256
#define FREQUENCY_LIST_DESCRIPTOR_INFO_MAX 64

//=============================================== 13818描述子：===============================================

typedef struct VIDEO_STREAM_DESCRIPTOR_T
{
	unsigned int uiDescriptor_tag :8;
	unsigned int uiDescriptor_length :8;
	unsigned int uiMultiple_frame_rate_flag :1;
	unsigned int uiFrame_rate_code :4;
	unsigned int uiMPEG_2_flag :1;
	unsigned int uiConstrained_parameter_flag :1;
	unsigned int uiStill_picture_flag :1;
	
	unsigned int uiProfile_and_level_indication :8;
	unsigned int uiChroma_format :2;
	unsigned int uiFrame_rate_extension_flag :1;
	unsigned int uiReserved :5;
} VIDEO_STREAM_DESCRIPTOR_T;

typedef struct AUDIO_STREAM_DESCRIPTOR_T
{
	unsigned int uiDescriptor_tag :8;
	unsigned int uiDescriptor_length :8;
	unsigned int uiFree_format_flag :1;
	unsigned int uiID :1;
	unsigned int uiLayer :2;
	unsigned int uiReserved :4;
} AUDIO_STREAM_DESCRIPTOR_T;

typedef struct CA_DESCRIPTOR_T
{
	unsigned int uiDescriptor_tag :8;
	unsigned int uiDescriptor_length :8;
	unsigned int uiCA_system_ID :16;
	unsigned int uiReserved :3;
	unsigned int uiCA_PID :13;
	unsigned char aucPrivate_data_byte[CA_DESCRIPTOR_INFO_MAX];
} CA_DESCRIPTOR_T;

typedef struct SMOOTHING_BUFFER_DESCRIPTOR_T
{
	unsigned int uiDescriptor_tag :8;
	unsigned int uiDescriptor_length :8;
	unsigned int uiExternal_clock_reference_indicator :1;
	unsigned int uiReserved_first :1;
	unsigned int uiClock_accuracy_integer :6;
	unsigned int uiClock_accuracy_exponent :3;
	unsigned int uiReserved_second :5;
} SMOOTHING_BUFFER_DESCRIPTOR_T;

typedef struct SYSTEM_CLOCK_DESCRIPTOR_T
{
	unsigned int uiDescriptor_tag :8;
	unsigned int uiDescriptor_length :8;
	unsigned int uiExternal_clock_reference_indicator :1;
	unsigned int uiReserved_first :1;
	unsigned int uiClock_accuracy_integer :6;
	unsigned int uiClock_accuracy_exponent :3;
	unsigned int uiReserved_second :5;
} SYSTEM_CLOCK_DESCRIPTOR_T;

typedef struct DATA_STREAM_ALIGNMENT_DESCRIPTOR_T
{
	unsigned int uiDescriptor_tag :8;
	unsigned int uiDescriptor_length :8;
	unsigned int uiAlignment_type :8;
} DATA_STREAM_ALIGNMENT_DESCRIPTOR_T;

typedef struct MAXIMUM_BITRATE_DESCRIPTOR_T
{
	unsigned int uiDescriptor_tag :8;
	unsigned int uiDescriptor_length :8;
	unsigned int uiReserved :2;
	unsigned int uiMaximum_bitrate :24;
} MAXIMUM_BITRATE_DESCRIPTOR_T;

typedef struct ISO_639_LANGUAGE_CODE_T
{
	unsigned char aucPrivate_data_byte[3];
} ISO_639_LANGUAGE_CODE_T;

typedef struct ISO_639_LANGUAGE_DESCRIPTOR_T
{
	unsigned int uiDescriptor_tag :8;
	unsigned int uiDescriptor_length :8;
	ISO_639_LANGUAGE_CODE_T stISO_639_Language_code[ISO_639_LANGUAGE_CODE_MAX];
	unsigned int uiAudio_type :8;
} ISO_639_LANGUAGE_DESCRIPTOR_T;

//=============================================== 300468描述子：===============================================
typedef struct NETWORK_NAME_DESCRIPTOR_T
{
	unsigned int uiDescriptor_tag :8;
	unsigned int uiDescriptor_length :8;
	unsigned char aucInfo[NETWORK_NAME_DESCRIPTOR_INFO_MAX];
} NETWORK_NAME_DESCRIPTOR_T;

typedef struct SERVICE_LIST_INFO_T
{
	unsigned int uiServiec_id :16;
	unsigned int uiService_type :8;
} SERVICE_LIST_INFO_T;

typedef struct SERVICE_LIST_DESCRIPTOR_T
{
	unsigned int uiDescriptor_tag :8;
	unsigned int uiDescriptor_length :8;
	SERVICE_LIST_INFO_T astService_info[SERVICE_LIST_DESCRIPTOR_INFO_MAX];
} SERVICE_LIST_DESCRIPTOR_T;

typedef struct SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR_T
{
	unsigned int uiDescriptor_tag :8;
	unsigned int uiDescriptor_length :8;
	unsigned int uiFrequency :32;
	unsigned int uiOrbital_position :16;
	unsigned int uiWest_east_flag :1;
	unsigned int uiPolarization :2;
	unsigned int uiRoll_off :2;
	unsigned int uiModulation_system :1;
	unsigned int uiModulation_type :2;
	unsigned int uiSymbol_rate :28;
	unsigned int uiFEC_inner :4;
} SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR_T;

typedef struct CABLE_DELIVERY_SYSTEM_DESCRIPTOR_T
{
	unsigned int uiDescriptor_tag :8;
	unsigned int uiDescriptor_length :8;
	unsigned int uiFrequency :32;
	unsigned int uiReserved_future_use :12;
	unsigned int uiFec_outer :4;
	unsigned int uiModulation :8;
	unsigned int uiSymbol_rate :28;
	unsigned int uiFEC_inner :4;
} CABLE_DELIVERY_SYSTEM_DESCRIPTOR_T;

typedef struct TERRESTRIAL_DELIVERY_SYSTEM_DESCRIPTOR_T
{
	unsigned int uiDescriptor_tag :8;
	unsigned int uiDescriptor_length :8;
	unsigned int uiCentre_frequency :32;
	unsigned int uiBandwidth :3;
	unsigned int uiPriority :1;
	unsigned int uiTime_Slicing_indicator :1;
	unsigned int uiMPE_FEC_indicator :1;
	unsigned int uiReserved_future_use_first :2;
	unsigned int uiConstellation :2;
	unsigned int uiHierarchy_information :3;
	unsigned int uiCode_rate_HP_stream :3;
	unsigned int uiCode_rate_LP_stream :3;
	unsigned int uiGuard_interval :2;
	unsigned int uiTransmission_mode :2;
	unsigned int uiOther_frequency_flag :1;
	unsigned int uiReserved_future_use_second :32;
} TERRESTRIAL_DELIVERY_SYSTEM_DESCRIPTOR_T;

typedef struct BOUQUET_NAME_DESCRIPTOR_T
{
	unsigned int uiDescriptor_tag :8;
	unsigned int uiDescriptor_length :8;
	unsigned char aucInfo[BOUQUET_NAME_DESCRIPTOR_INFO_MAX];
} BOUQUET_NAME_DESCRIPTOR_T;

typedef struct SERVICE_DESCRIPTOR_T
{
	unsigned int uiDescriptor_tag :8;
	unsigned int uiDescriptor_length :8;
	unsigned int uiService_type :8;
	unsigned int uiService_provider_name_length :8;
	unsigned char aucService_provider_name[SERVICE_DESCRIPTOR_INFO_MAX];
	unsigned int uiService_name_length :8;
	unsigned char aucService_name[SERVICE_DESCRIPTOR_INFO_MAX];
} SERVICE_DESCRIPTOR_T;

typedef struct LINKAGE_UNION_08_T
{
	unsigned int uiHand_over_type :4;
	unsigned int uiReserved_future_use :3;
	unsigned int uiOrigin_type :1;
	unsigned int uiNetwork_id :16;
	unsigned int uiInitial_service_id :16;
} LINKAGE_UNION_08_T;

typedef struct LINKAGE_UNION_0D_T
{
	unsigned int uiTarget_event_id :16;
	unsigned int uiTarget_listed :1;
	unsigned int uiEvent_simulcast :1;
	unsigned int uiReserved :6;
} LINKAGE_UNION_0D_T;

typedef struct LINKAGE_DESCRIPTOR_T
{
	unsigned int uiDescriptor_tag :8;
	unsigned int uiDescriptor_length :8;
	unsigned int uiTransport_stream_id :16;
	unsigned int uiOriginal_network_id :16;
	unsigned int uiService_id :16;
	unsigned int uiLinage_type :8;
	union LINKAGE_INFO_UNION_T
	{
		LINKAGE_UNION_08_T stLinkage_union_08;
		LINKAGE_UNION_0D_T stLinkage_union_0d;
	} stLinkage_info_union;
	unsigned char aucPrivate_data[LINKAGE_DESCRIPTOR_INFO_MAX];
} LINKAGE_DESCRIPTOR_T;

typedef struct STREAM_IDENTIFIER_DESCRIPTOR_T
{
	unsigned int uiDescriptor_tag :8;
	unsigned int uiDescriptor_length :8;
	unsigned int uiComponent_tag :8;
} STREAM_IDENTIFIER_DESCRIPTOR_T;

typedef struct TELETEXT_INFO_T
{
	ISO_639_LANGUAGE_CODE_T stISO_639_Language_code;
	unsigned int uiTeletext_type :5;
	unsigned int uiTeletext_magazine_number :3;
	unsigned int uiTeletext_page_number :8;
} TELETEXT_INFO_T;

typedef struct TELETEXT_DESCRIPTOR_T
{
	unsigned int uiDescriptor_tag :8;
	unsigned int uiDescriptor_length :8;
	TELETEXT_INFO_T astTeletext_Info[TELETEXT_INFO_MAX];
} TELETEXT_DESCRIPTOR_T;

typedef struct SUBTITLING_INFO_T
{
	ISO_639_LANGUAGE_CODE_T stISO_639_Language_code;
	unsigned int uiSubtitling_type :8;
	unsigned int uiComposition_page_id :16;
	unsigned int uiAncillary_page_id :16;
} SUBTITLING_INFO_T;

typedef struct SUBTITLING_DESCRIPTOR_T
{
	unsigned int uiDescriptor_tag :8;
	unsigned int uiDescriptor_length :8;
	SUBTITLING_INFO_T astSubtitling_Info[SUBTITLING_INFO_MAX];
} SUBTITLING_DESCRIPTOR_T;

typedef struct SHORT_EVENT_DESCRIPTOR_T
{
	unsigned int uiDescriptor_tag :8;
	unsigned int uiDescriptor_length :8;
	ISO_639_LANGUAGE_CODE_T stISO_639_Language_code;
	unsigned int uiEvent_name_length :8;
	unsigned char aucEvent_name_char[SHORT_EVENT_INFO_MAX];
	unsigned int uiText_length :8;
	unsigned char aucText_char[SHORT_EVENT_INFO_MAX];
} SHORT_EVENT_DESCRIPTOR_T;

typedef struct EXTENDED_EVENT_INFO_T
{
	unsigned int uiItem_descriptor_length :8;
	unsigned char aucItem_descriptor_char[EXTENDED_EVENT_DESCRIPTOR_CHAR_MAX];
	unsigned int uiItem_length :8;
	unsigned char aucItem_char[EXTENDED_EVENT_DESCRIPTOR_CHAR_MAX];
} EXTENDED_EVENT_INFO_T;

typedef struct EXTENDED_EVENT_DESCRIPTOR_T
{
	unsigned int uiDescriptor_tag :8;
	unsigned int uiDescriptor_length :8;
	unsigned int uiDescriptor_number :4;
	unsigned int uiLast_descriptor_number :4;
	ISO_639_LANGUAGE_CODE_T stISO_639_language_code;
	unsigned int uiLength_of_items :8;
	EXTENDED_EVENT_INFO_T astExtended_event_info[EXTENDED_EVENT_DESCRIPTOR_INFO_MAX];
	unsigned int uiText_length :8;
	unsigned char aucText_char[EXTENDED_EVENT_DESCRIPTOR_CHAR_MAX];
} EXTENDED_EVENT_DESCRIPTOR_T;

typedef struct FREQUENCY_LIST_INFO_T
{
	unsigned int uiCentre_frequency :32;
} FREQUENCY_LIST_INFO_T;

typedef struct FREQUENCY_LIST_DESCRIPTOR_T
{
	unsigned int uiDescriptor_tag :8;
	unsigned int uiDescriptor_length :8;
	unsigned int uiReserved_future_use :6;
	unsigned int uiCoding_type :2;
	FREQUENCY_LIST_INFO_T astCentre_frequency[FREQUENCY_LIST_DESCRIPTOR_INFO_MAX];
} FREQUENCY_LIST_DESCRIPTOR_T;

int GetFrequencyListDescriptor(FREQUENCY_LIST_DESCRIPTOR_T *pstFrequencyListDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition);
int GetTerrestrialDeliverySystemDescriptor(TERRESTRIAL_DELIVERY_SYSTEM_DESCRIPTOR_T *pstTerrestrialDeliverySystemDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition);
int GetExtendedEventDescriptor(EXTENDED_EVENT_DESCRIPTOR_T *pstExtendedEventDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition);
int GetShortEventDescriptor(SHORT_EVENT_DESCRIPTOR_T *pstShortEventDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition);
int GetLinkageDescriptor(LINKAGE_DESCRIPTOR_T *pstLinkageDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition);
int GetServiceDescriptor(SERVICE_DESCRIPTOR_T *pstServiceDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition);
int GetBouquetNameDescriptor(BOUQUET_NAME_DESCRIPTOR_T *pstBouquetNameDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition);
int GetCableDeliverySystemDescriptor(CABLE_DELIVERY_SYSTEM_DESCRIPTOR_T *pstCableDeliverySystemDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition);
int GetSatelliteDeliverySystemDescriptor(SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR_T *pstSatelliteDeliverySystemDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition);
int GetServiceListDescriptor(SERVICE_LIST_DESCRIPTOR_T *pstServiceListDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition);
int GetNetworkNameDescriptor(NETWORK_NAME_DESCRIPTOR_T *pstNetworkNameDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition);
int GetMaximumBitrateDescriptor(MAXIMUM_BITRATE_DESCRIPTOR_T *pstMaximumBitrateDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition);
int GetSystemClockDescriptor(SYSTEM_CLOCK_DESCRIPTOR_T *pstSystemClockDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition);
int GetCA_Descriptor(CA_DESCRIPTOR_T *pstCA_Descriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition);
int GetStreamIndentifierDescriptor(STREAM_IDENTIFIER_DESCRIPTOR_T *pstStreamIndentifierDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition);
int GetVideoStreamDescriptor(VIDEO_STREAM_DESCRIPTOR_T *pstVideoStreamDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition);
int GetDataStreamAlignmentDescriptor(DATA_STREAM_ALIGNMENT_DESCRIPTOR_T *pstDataStreamAlignmentDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition);
int GetISO_639_Language_Descriptor(ISO_639_LANGUAGE_DESCRIPTOR_T *pstISO_639_LanguageDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition);
int GetTeletextDescriptor(TELETEXT_DESCRIPTOR_T *pstTeletextDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition);
int GetSubtitlingDescriptor(SUBTITLING_DESCRIPTOR_T *pstSubtitlingDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition);
int GetAudioStreamDescriptor(AUDIO_STREAM_DESCRIPTOR_T *pstAudioStreamDescriptor, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iDescriptorPosition);
#endif
