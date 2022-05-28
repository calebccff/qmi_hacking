/*
 * Copyright (C) 2022 Caleb Connolly <caleb.connolly@linaro.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <linux/qrtr.h>
#include <memory.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>

#include "qmi_qrild_svc.h"

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

static uint8_t to_hex(uint8_t ch) {
	ch &= 0xf;
	return ch <= 9 ? '0' + ch : 'a' + ch - 10;
}

#define LINE_LENGTH 48

void print_hex_dump(const char *prefix, const void *buf, size_t len) {
	const uint8_t *ptr = buf;
	size_t linelen;
	uint8_t ch;
	char line[LINE_LENGTH * 4 + 1];
	int li;
	int i;
	int j;

	if (len < 0) {
		printf("%s: len %zu less than 0", __func__, len);
		return;
	}

	if (prefix)
		printf("<<< %s:\n", prefix);

	for (i = 0; i < len; i += LINE_LENGTH) {
		linelen = MIN(LINE_LENGTH, len - i);
		li = 0;

		for (j = 0; j < linelen; j++) {
			ch = ptr[i + j];
			line[li++] = to_hex(ch >> 4);
			line[li++] = to_hex(ch);
			line[li++] = ' ';
		}

		for (; j < LINE_LENGTH; j++) {
			line[li++] = ' ';
			line[li++] = ' ';
			line[li++] = ' ';
		}

		// for (j = 0; j < linelen; j++) {
		// 	ch = ptr[i + j];
		// 	line[li++] = isprint(ch) ? ch : '.';
		// }

		line[li] = '\0';

		printf("<<< %04x: %s (%zu)\n", i, line, linelen);
	}
}

void dump_card_status(struct qrild_svc_uim_card_status *cs) {
	int i, ii, iii;
	printf("index_gw_primary: %u, ", cs->index_gw_primary);
	printf("index_1x_primary: %u, ", cs->index_1x_primary);
	printf("index_gw_secondary: %u, ", cs->index_gw_secondary);
	printf("index_1x_secondary: %u, ", cs->index_1x_secondary);
	printf("cards_n: %u", cs->cards_n);

	for (i = 0; i < cs->cards_n; i++) {
		printf("\ncard %d:\n\t", i);
		printf("card_state: %u, ", cs->cards[i].card_state);
		printf("upin_state: %u, ", cs->cards[i].upin_state);
		printf("upin_retries: %u, ", cs->cards[i].upin_retries);
		printf("upuk_retries: %u, ", cs->cards[i].upuk_retries);
		printf("error_code: %u, ", cs->cards[i].error_code);
		printf("applications_n: %u, ", cs->cards[i].applications_n);
		for (ii = 0; ii < cs->cards[i].applications_n; ii++) {
			printf("\n\tapplication %d:\n\t\t", ii);
			printf("type: %u, ",
			       cs->cards[i].applications[ii].type);
			printf("state: %u, ",
			       cs->cards[i].applications[ii].state);
			printf("personalization_state: %u, ",
			       cs->cards[i]
				   .applications[ii]
				   .personalization_state);
			printf("personalization_feature: %u, ",
			       cs->cards[i]
				   .applications[ii]
				   .personalization_feature);
			printf("personalization_retries: %u, ",
			       cs->cards[i]
				   .applications[ii]
				   .personalization_retries);
			printf("application_identifier_value_n: %u, ",
			       cs->cards[i]
				   .applications[ii]
				   .application_identifier_value_n);
			printf("\n\t\tapplication identifier\n\t\t\t");
			for (iii = 0; iii < cs->cards[i]
						.applications[ii]
						.application_identifier_value_n;
			     iii++) {
				printf("[%u], ",
				       cs->cards[i]
					   .applications[ii]
					   .application_identifier_value[iii]);
			}
			printf("\n\t\t");
			printf("pin2_state: %u, ",
			       cs->cards[i].applications[ii].pin2_state);
			printf("pin2_retries: %u, ",
			       cs->cards[i].applications[ii].pin2_retries);
			printf("puk2_retries: %u, ",
			       cs->cards[i].applications[ii].puk2_retries);
		}
	}
	printf("\n");
}

int main(int argc, char **argv) {
	struct qrild_svc_uim_get_card_status_resp *resp;
	struct qrild_svc_uim_card_status *cs;
	int rc;

	resp = qrild_svc_uim_get_card_status_resp_alloc(1);

	cs = malloc(sizeof(struct qrild_svc_uim_card_status));
	cs->cards_n = 2;
	cs->index_gw_primary = 0x3f7b;
	cs->index_1x_primary = 0xa35c;
	cs->index_gw_secondary = 0xa48a;
	cs->index_1x_secondary = 0xa53e;

	cs->cards = malloc(sizeof(struct qrild_svc_uim_card_status_cards) *
			   cs->cards_n);

	for (int i = 0; i < cs->cards_n; i++) {
		cs->cards[i].card_state = (i << 4) + 1;
		cs->cards[i].upin_state = (i << 4) + 2;
		cs->cards[i].upin_retries = (i << 4) + 3;
		cs->cards[i].upuk_retries = (i << 4) + 4;
		cs->cards[i].error_code = (i << 4) + 5;
		cs->cards[i].applications_n = 1;
		cs->cards[i].applications = malloc(sizeof(
			struct qrild_svc_uim_card_status_cards_applications) *
		    cs->cards[i].applications_n);
		for (int ii = 0; ii < cs->cards[i].applications_n; ii++) {
			printf("Creating appn %d (%p)\n", ii, cs->cards[i].applications);
			cs->cards[i].applications[ii].type = (ii << 4) + 1;
			cs->cards[i].applications[ii].state = (ii << 4) + 2;
			cs->cards[i].applications[ii].personalization_state = (ii << 4) + 4;
			cs->cards[i].applications[ii].personalization_feature = (ii << 4) + 8;
			cs->cards[i].applications[ii].personalization_retries = (ii << 4) + 16;
			cs->cards[i]
			    .applications[ii]
			    .application_identifier_value_n = 16;

			cs->cards[i]
			    .applications[ii]
			    .application_identifier_value =
			    malloc(sizeof(uint8_t) *
				   cs->cards[i]
				       .applications[ii]
				       .application_identifier_value_n);
			for (int iii = 0;
			     iii < cs->cards[i]
				       .applications[ii]
				       .application_identifier_value_n;
			     iii++) {

				cs->cards[i]
				    .applications[ii]
				    .application_identifier_value[iii] = iii;
			}
			cs->cards[i].applications[ii].pin2_state = (i + ii) + 3;
			cs->cards[i].applications[ii].pin2_retries = (i + ii) + 2;
			cs->cards[i].applications[ii].puk2_retries = (i + ii) + 32;
		}
	}

	printf("Generated fake UIM card status:\n");
	dump_card_status(cs);
	printf("Top level struct as binary:\n");
	print_hex_dump("UIM", cs, sizeof(struct qrild_svc_uim_card_status));
	//printf("First card as binary:\n");
	//print_hex_dump("CARD", cs->cards, sizeof(struct qrild_svc_uim_card_status_cards));
	size_t serialised_len;
	rc = qrild_svc_uim_get_card_status_resp_set_status(resp, cs,
							   &serialised_len);
	printf("\nSerialised into %zu bytes, rc=%d\n", serialised_len, rc);
	void *buf = qmi_tlv_get((struct qmi_tlv *)resp, 16, &serialised_len);
	printf("Raw qmi_tlv_get, %zu bytes\n", serialised_len);
	print_hex_dump("Serialised bytes:", buf, serialised_len);
	free(cs);
	cs = qrild_svc_uim_get_card_status_resp_get_status(resp);
	printf("Deserialised get()\n");
	print_hex_dump("UIM", cs, sizeof(struct qrild_svc_uim_card_status));
	dump_card_status(cs);

	return 0;
}
