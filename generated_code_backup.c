#define get_n(_ptr, _type) \ ({
	_type v = 0; \
	int i; \
	for(i = 0; i < sizeof(type); i++) \
		v |= ((*(_ptr)++) << i)
	v;
})

int qrild_svc_uim_get_card_status_resp_set_status(struct qrild_svc_uim_get_card_status_resp *uim_get_card_status_resp, struct qrild_svc_uim_card_status *val, size_t *len_out)
{
	uint8_t* data = malloc(2048);
	size_t amt = offsetof(struct qrild_svc_uim_card_status, cards);
	size_t len = 0;
	size_t i;

	memcpy(data, val, amt);
	len += amt;


	for(i = 0; i < val->cards_n; i++) {
		size_t cards_sz = sizeof(val->cards[0]);
		size_t ii;
		amt = offsetof(struct qrild_svc_uim_card_status_cards, applications);
		printf("card %zu, len %2zu, copying %2zu bytes\n", i, len, amt);
		memcpy(data+len, val->cards + cards_sz * i, amt);
		len += amt;
		for(ii = 0; ii < val->cards[i].applications_n; ii++) {
			size_t applications_sz = sizeof(val->cards[0].applications[0]);
			size_t iii;
			amt = offsetof(struct qrild_svc_uim_card_status_cards_applications, application_identifier_value);
			printf("application %zu, len %2zu, copying %2zu bytes\n", ii, len, amt);
			memcpy(data + len, val->cards[i].applications + applications_sz * ii, amt);
			len += amt;
			for(iii = 0; iii < val->cards[i].applications[ii].application_identifier_value_n; iii++) {
				amt = sizeof(val->cards[0].applications[0].application_identifier_value[0]);
				memcpy(data + len, val->cards[i].applications[ii].application_identifier_value + iii * amt, amt);
				len += amt;
			}
			amt = offsetof(struct qrild_svc_uim_card_status_cards_applications, puk2_retries) - offsetof(struct qrild_svc_uim_card_status_cards_applications, upin_replaces_pin1);
			printf("application %zu, len %2zu, copying %2zu bytes (upin_replaces_pin1)\n", ii, len, amt);
			memcpy(data + len, val->cards[i].applications + sizeof(struct qrild_svc_uim_card_status_cards_applications) * ii + offsetof(struct qrild_svc_uim_card_status_cards_applications, upin_replaces_pin1), amt);
			len += amt;
		}
	}

	*len_out = len;

	return qmi_tlv_set((struct qmi_tlv*)uim_get_card_status_resp, 16, data, len);
}

struct qrild_svc_uim_card_status *qrild_svc_uim_get_card_status_resp_get_status(struct qrild_svc_uim_get_card_status_resp *uim_get_card_status_resp)
{
	size_t len;
	void *ptr, cur;
	struct qrild_svc_uim_card_status *cs;

	ptr = qmi_tlv_get((struct qmi_tlv*)uim_get_card_status_resp, 16, &len);
	if (!ptr)
		return NULL;
	
	cur = ptr;

	cs = malloc(sizeof(struct qrild_svc_uim_card_status));
	cs->index_gw_primary = get_n(cur, uint16_t);
	cs->index_1x_primary = get_n(cut, uint16_t);
	cs->index_gw_secondary = get_n(cut, uint16_t);
	cs->index_1x_secondary = *cur++;
	cs->cards_n = 

	return ptr;
}
int qrild_svc_uim_get_card_status_resp_set_status(struct qrild_svc_uim_get_card_status_resp *uim_get_card_status_resp, struct qrild_svc_uim_card_status *val, size_t *len_out)
{
	uint8_t* data = malloc(2048);
	size_t amt = offsetof(struct qrild_svc_uim_card_status, cards);
	size_t len = 0;
	size_t i;

	memcpy(data, val, amt);
	len += amt;


	for(i = 0; i < val->cards_n; i++) {
		size_t cards_sz = sizeof(val->cards[0]);
		size_t ii;
		amt = offsetof(struct qrild_svc_uim_card_status_cards, applications);
		printf("card %zu, len %2zu, copying %2zu bytes\n", i, len, amt);
		memcpy(data+len, val->cards + cards_sz * i, amt);
		len += amt;
		for(ii = 0; ii < val->cards[i].applications_n; ii++) {
			size_t applications_sz = sizeof(val->cards[0].applications[0]);
			size_t iii;
			amt = offsetof(struct qrild_svc_uim_card_status_cards_applications, application_identifier_value);
			printf("application %zu, len %2zu, copying %2zu bytes\n", ii, len, amt);
			memcpy(data + len, val->cards[i].applications + applications_sz * ii, amt);
			len += amt;
			for(iii = 0; iii < val->cards[i].applications[ii].application_identifier_value_n; iii++) {
				amt = sizeof(val->cards[0].applications[0].application_identifier_value[0]);
				memcpy(data + len, val->cards[i].applications[ii].application_identifier_value + iii * amt, amt);
				len += amt;
			}
			amt = offsetof(struct qrild_svc_uim_card_status_cards_applications, puk2_retries) - offsetof(struct qrild_svc_uim_card_status_cards_applications, upin_replaces_pin1);
			printf("application %zu, len %2zu, copying %2zu bytes (upin_replaces_pin1)\n", ii, len, amt);
			memcpy(data + len, val->cards[i].applications + sizeof(struct qrild_svc_uim_card_status_cards_applications) * ii + offsetof(struct qrild_svc_uim_card_status_cards_applications, upin_replaces_pin1), amt);
			len += amt;
		}
	}

	*len_out = len;

	return qmi_tlv_set((struct qmi_tlv*)uim_get_card_status_resp, 16, data, len);
}

struct qrild_svc_uim_card_status *qrild_svc_uim_get_card_status_resp_get_status(struct qrild_svc_uim_get_card_status_resp *uim_get_card_status_resp)
{
	size_t len;
	void *ptr, cur;
	struct qrild_svc_uim_card_status *cs;

	ptr = qmi_tlv_get((struct qmi_tlv*)uim_get_card_status_resp, 16, &len);
	if (!ptr)
		return NULL;
	
	cur = ptr;

	cs = malloc(sizeof(struct qrild_svc_uim_card_status));
	cs->index_gw_primary = get_n(cur, uint16_t);
	cs->index_1x_primary = get_n(cut, uint16_t);
	cs->index_gw_secondary = get_n(cut, uint16_t);
	cs->index_1x_secondary = *cur++;
	cs->cards_n = 

	return ptr;
}
