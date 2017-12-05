{
  'targets': [
    {
      'target_name': 'fs_directory_stream',
      'include_dirs' : [
        "<!(node -e \"require('nan')\")",
      ],
      'sources': [
        'binding.cc',
      ],
      'xcode_settings': {
        'OTHER_CFLAGS': [
          '-O3',
        ]
      },
      'cflags': [
        '-O3',
      ],
    }
  ]
}
