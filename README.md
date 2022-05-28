# QMI test

Testing ground for QMI/qmic stuff. the code in `generated_code_backup.c` can
serialise and de-serialise the `qrild_svc_uim_card_status` struct which
represents a TLV included in the "UIM get card status" QMI response. The next
step is to extend QMIC (the QMI IDL parser) to generate setters/getters in a
similar format.