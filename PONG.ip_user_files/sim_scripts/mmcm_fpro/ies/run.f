-makelib ies_lib/xpm -sv \
  "C:/Xilinx/Vivado/2019.2/data/ip/xpm/xpm_cdc/hdl/xpm_cdc.sv" \
  "C:/Xilinx/Vivado/2019.2/data/ip/xpm/xpm_memory/hdl/xpm_memory.sv" \
-endlib
-makelib ies_lib/xpm \
  "C:/Xilinx/Vivado/2019.2/data/ip/xpm/xpm_VCOMP.vhd" \
-endlib
-makelib ies_lib/xil_defaultlib \
  "../../../../PONG.srcs/sources_1/ip/mmcm_fpro/mmcm_fpro_clk_wiz.v" \
  "../../../../PONG.srcs/sources_1/ip/mmcm_fpro/mmcm_fpro.v" \
-endlib
-makelib ies_lib/xil_defaultlib \
  glbl.v
-endlib

