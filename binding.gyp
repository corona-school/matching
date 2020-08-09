{
    'targets': [
      {
        'target_name': 'matching',
        'defines': [
          'DEFINE_FOO',
          'DEFINE_A_VALUE=value',
          'NAPI_DISABLE_CPP_EXCEPTIONS',
        ],
        'include_dirs': [
          "<!@(node -p \"require('node-addon-api').include\")",
        ],
        'sources': [
          'src/matching_algo/index.cpp', 
          'src/matching_algo/src/graph.cpp', 
          'src/matching_algo/src/student.cpp', 
          'src/matching_algo/src/algo.cpp',
        ],
        "cflags!": [ "-fno-exceptions" ],
        "cflags_cc!": [ "-fno-exceptions" ],
        'cflags_cc': [
          '-std=c++17', # use c++17
        ],
        'conditions': [
          ['OS=="linux"', {
            'defines': [
              'LINUX_DEFINE',
            ],
            'include_dirs': [
              'include/linux',
            ],
          }],
          ['OS=="mac"', {
            'defines': [
              'MAC_DEFINE',
            ],
            'xcode_settings': { # define special things for macOS (in particular, without that, c++17 wouldn't work on macOS)
              'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
              'MACOSX_DEPLOYMENT_TARGET': '10.15',
              'CLANG_CXX_LANGUAGE_STANDARD': 'c++17',
              'CLANG_CXX_LIBRARY': 'libc++',
            },
          }],
          ['OS=="win"', {
            'defines': [
              'WINDOWS_SPECIFIC_DEFINE',
            ],
          }, { # OS != "win",
            'defines': [
              'NON_WINDOWS_DEFINE',
            ],
          }]
        ],
      },
    ],
  }
