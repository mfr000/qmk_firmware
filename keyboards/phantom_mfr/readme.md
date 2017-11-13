Phantom
=======

A community-developed keyboard PCB designed to fit inside the case of a Filco Majestouch.

See the [Deskthority wiki](https://deskthority.net/wiki/Phantom) for more information.

Keyboard Maintainer: QMK Community
Hardware Supported: Phantom PCB
Hardware Availability: https://mechanicalkeyboards.com/shop/index.php?l=product_detail&p=536

Make example for this keyboard (after setting up your build environment):

    make phantom:default

See [build environment setup](https://docs.qmk.fm/build_environment_setup.html) then the [make instructions](https://docs.qmk.fm/make_instructions.html) for more information.

==== @@@ MFR ====
To build for chibios, use:

   make phantom_mfr:default:dfu-util
   dfu-util -D -d xxx:xxx -a 1 .build/phantom_mfr_default.bin
