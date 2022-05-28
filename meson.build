project('qmi_test', 'c')

deps = [
]

src = [
        'qmi_test.c',
        'qmi_tlv.c',
        'qmi_qrild_svc.c',
]

inc = [
        include_directories('.'),
]

executable('qmi_test', src,
        include_directories: inc,
        dependencies: deps,
        install: true,
        c_args: ['-fpack-struct'],
)
