
module openC906 ( axim_clk_en, biu_pad_araddr, biu_pad_arburst, 
        biu_pad_arcache, biu_pad_arid, biu_pad_arlen, biu_pad_arlock, 
        biu_pad_arprot, biu_pad_arsize, biu_pad_arvalid, biu_pad_awaddr, 
        biu_pad_awburst, biu_pad_awcache, biu_pad_awid, biu_pad_awlen, 
        biu_pad_awlock, biu_pad_awprot, biu_pad_awsize, biu_pad_awvalid, 
        biu_pad_bready, biu_pad_rready, biu_pad_wdata, biu_pad_wlast, 
        biu_pad_wstrb, biu_pad_wvalid, core0_pad_halted, core0_pad_lpmd_b, 
        core0_pad_retire, core0_pad_retire_pc, cpu_debug_port, pad_biu_arready, 
        pad_biu_awready, pad_biu_bid, pad_biu_bresp, pad_biu_bvalid, 
        pad_biu_rdata, pad_biu_rid, pad_biu_rlast, pad_biu_rresp, 
        pad_biu_rvalid, pad_biu_wready, pad_cpu_apb_base, pad_cpu_rst_b, 
        pad_cpu_rvba, pad_cpu_sys_cnt, pad_plic_int_cfg, pad_plic_int_vld, 
        pad_tdt_dm_arready, pad_tdt_dm_awready, pad_tdt_dm_bid, 
        pad_tdt_dm_bresp, pad_tdt_dm_bvalid, pad_tdt_dm_core_unavail, 
        pad_tdt_dm_rdata, pad_tdt_dm_rid, pad_tdt_dm_rlast, pad_tdt_dm_rresp, 
        pad_tdt_dm_rvalid, pad_tdt_dm_wready, pad_yy_dft_clk_rst_b, 
        pad_yy_icg_scan_en, pad_yy_mbist_mode, pad_yy_scan_enable, 
        pad_yy_scan_mode, pad_yy_scan_rst_b, pll_core_cpuclk, sys_apb_clk, 
        sys_apb_rst_b, tdt_dm_pad_araddr, tdt_dm_pad_arburst, 
        tdt_dm_pad_arcache, tdt_dm_pad_arid, tdt_dm_pad_arlen, 
        tdt_dm_pad_arlock, tdt_dm_pad_arprot, tdt_dm_pad_arsize, 
        tdt_dm_pad_arvalid, tdt_dm_pad_awaddr, tdt_dm_pad_awburst, 
        tdt_dm_pad_awcache, tdt_dm_pad_awid, tdt_dm_pad_awlen, 
        tdt_dm_pad_awlock, tdt_dm_pad_awprot, tdt_dm_pad_awsize, 
        tdt_dm_pad_awvalid, tdt_dm_pad_bready, tdt_dm_pad_hartreset_n, 
        tdt_dm_pad_ndmreset_n, tdt_dm_pad_rready, tdt_dm_pad_wdata, 
        tdt_dm_pad_wlast, tdt_dm_pad_wstrb, tdt_dm_pad_wvalid, tdt_dmi_paddr, 
        tdt_dmi_penable, tdt_dmi_prdata, tdt_dmi_pready, tdt_dmi_psel, 
        tdt_dmi_pslverr, tdt_dmi_pwdata, tdt_dmi_pwrite );
  output [39:0] biu_pad_araddr;
  output [1:0] biu_pad_arburst;
  output [3:0] biu_pad_arcache;
  output [7:0] biu_pad_arid;
  output [7:0] biu_pad_arlen;
  output [2:0] biu_pad_arprot;
  output [2:0] biu_pad_arsize;
  output [39:0] biu_pad_awaddr;
  output [1:0] biu_pad_awburst;
  output [3:0] biu_pad_awcache;
  output [7:0] biu_pad_awid;
  output [7:0] biu_pad_awlen;
  output [2:0] biu_pad_awprot;
  output [2:0] biu_pad_awsize;
  output [127:0] biu_pad_wdata;
  output [15:0] biu_pad_wstrb;
  output [1:0] core0_pad_lpmd_b;
  output [39:0] core0_pad_retire_pc;
  input [7:0] pad_biu_bid;
  input [1:0] pad_biu_bresp;
  input [127:0] pad_biu_rdata;
  input [7:0] pad_biu_rid;
  input [1:0] pad_biu_rresp;
  input [39:0] pad_cpu_apb_base;
  input [39:0] pad_cpu_rvba;
  input [63:0] pad_cpu_sys_cnt;
  input [239:0] pad_plic_int_cfg;
  input [239:0] pad_plic_int_vld;
  input [3:0] pad_tdt_dm_bid;
  input [1:0] pad_tdt_dm_bresp;
  input [127:0] pad_tdt_dm_rdata;
  input [3:0] pad_tdt_dm_rid;
  input [1:0] pad_tdt_dm_rresp;
  output [39:0] tdt_dm_pad_araddr;
  output [1:0] tdt_dm_pad_arburst;
  output [3:0] tdt_dm_pad_arcache;
  output [3:0] tdt_dm_pad_arid;
  output [3:0] tdt_dm_pad_arlen;
  output [2:0] tdt_dm_pad_arprot;
  output [2:0] tdt_dm_pad_arsize;
  output [39:0] tdt_dm_pad_awaddr;
  output [1:0] tdt_dm_pad_awburst;
  output [3:0] tdt_dm_pad_awcache;
  output [3:0] tdt_dm_pad_awid;
  output [3:0] tdt_dm_pad_awlen;
  output [2:0] tdt_dm_pad_awprot;
  output [2:0] tdt_dm_pad_awsize;
  output [127:0] tdt_dm_pad_wdata;
  output [15:0] tdt_dm_pad_wstrb;
  input [11:0] tdt_dmi_paddr;
  output [31:0] tdt_dmi_prdata;
  input [31:0] tdt_dmi_pwdata;
  input axim_clk_en, pad_biu_arready, pad_biu_awready, pad_biu_bvalid,
         pad_biu_rlast, pad_biu_rvalid, pad_biu_wready, pad_cpu_rst_b,
         pad_tdt_dm_arready, pad_tdt_dm_awready, pad_tdt_dm_bvalid,
         pad_tdt_dm_core_unavail, pad_tdt_dm_rlast, pad_tdt_dm_rvalid,
         pad_tdt_dm_wready, pad_yy_dft_clk_rst_b, pad_yy_icg_scan_en,
         pad_yy_mbist_mode, pad_yy_scan_enable, pad_yy_scan_mode,
         pad_yy_scan_rst_b, pll_core_cpuclk, sys_apb_clk, sys_apb_rst_b,
         tdt_dmi_penable, tdt_dmi_psel, tdt_dmi_pwrite;
  output biu_pad_arlock, biu_pad_arvalid, biu_pad_awlock, biu_pad_awvalid,
         biu_pad_bready, biu_pad_rready, biu_pad_wlast, biu_pad_wvalid,
         core0_pad_halted, core0_pad_retire, cpu_debug_port, tdt_dm_pad_arlock,
         tdt_dm_pad_arvalid, tdt_dm_pad_awlock, tdt_dm_pad_awvalid,
         tdt_dm_pad_bready, tdt_dm_pad_hartreset_n, tdt_dm_pad_ndmreset_n,
         tdt_dm_pad_rready, tdt_dm_pad_wlast, tdt_dm_pad_wvalid,
         tdt_dmi_pready, tdt_dmi_pslverr;
  wire   biu_ifu_arready, biu_ifu_rid, biu_ifu_rlast, biu_ifu_rvalid,
         biu_lsu_arready, biu_lsu_no_op, biu_lsu_rlast, biu_lsu_rvalid,
         biu_lsu_stb_awready, biu_lsu_stb_wready, biu_lsu_vb_awready,
         biu_lsu_vb_wready, cp0_biu_icg_en, core0_rst_b, dtu_tdt_dm_halted,
         dtu_tdt_dm_havereset, dtu_tdt_dm_itr_done,
         dtu_tdt_dm_retire_debug_expt_vld, dtu_tdt_dm_wr_ready, ifu_biu_arid,
         ifu_biu_arprot_0_, ifu_biu_arvalid, lsu_biu_arprot_0_, lsu_biu_aruser,
         lsu_biu_arvalid, lsu_biu_stb_awuser, lsu_biu_stb_awvalid,
         lsu_biu_stb_wlast, lsu_biu_stb_wvalid, lsu_biu_vb_awvalid,
         lsu_biu_vb_wlast, lsu_biu_vb_wvalid, sync_sys_apb_rst_b,
         sysio_core0_me_int, sysio_core0_ms_int, sysio_core0_mt_int,
         sysio_core0_se_int, sysio_core0_ss_int, sysio_core0_st_int,
         tdt_dm_dtu_ack_havereset, tdt_dm_dtu_async_halt_req,
         tdt_dm_dtu_halt_on_reset, tdt_dm_dtu_halt_req, tdt_dm_dtu_itr_vld,
         tdt_dm_dtu_resume_req, tdt_dm_dtu_wr_vld, apb_clk_en, axim_clk_en_f,
         ciu_rst_b, penable, perr_clint, perr_plic, pready_clint, pready_plic,
         psel_clint, psel_plic, pwrite, clkgen_rst_b, apb_clk,
         clint_core0_ms_int, clint_core0_mt_int, clint_core0_ss_int,
         clint_core0_st_int, plic_hartx_mint_req_0_, plic_hartx_sint_req_0_,
         biu_pad_bready, n2, n3, __UC__n_1,
         __UC__n_2, __UC__n_3,
         __UC__n_4, __UC__n_5,
         __UC__n_6, __UC__n_7,
         __UC__n_8, __UC__n_9,
         __UC__n_10, __UC__n_11,
         __UC__n_12, __UC__n_13,
         __UC__n_14, __UC__n_15,
         __UC__n_16, __UC__n_17,
         __UC__n_18, __UC__n_19,
         __UC__n_20, __UC__n_21,
         __UC__n_22, __UC__n_23,
         __UC__n_24, __UC__n_25,
         __UC__n_26, __UC__n_27,
         __UC__n_28, __UC__n_29,
         __UC__n_30, __UC__n_31,
         __UC__n_32, __UC__n_33,
         __UC__n_34, __UC__n_35,
         __UC__n_36, __UC__n_37,
         __UC__n_38, __UC__n_39,
         __UC__n_40, __UC__n_41,
         __UC__n_42, __UC__n_43,
         __UC__n_44, __UC__n_45,
         __UC__n_46, __UC__n_47,
         __UC__n_48, __UC__n_49,
         __UC__n_50, __UC__n_51,
         __UC__n_52, __UC__n_53,
         __UC__n_54, __UC__n_55,
         __UC__n_56, __UC__n_57,
         __UC__n_58, __UC__n_59,
         __UC__n_60, __UC__n_61,
         __UC__n_62, __UC__n_63,
         __UC__n_64, __UC__n_65,
         __UC__n_66, __UC__n_67,
         __UC__n_68, __UC__n_69,
         __UC__n_70, __UC__n_71,
         __UC__n_72, __UC__n_73,
         __UC__n_74, __UC__n_75,
         __UC__n_76, __UC__n_77,
         __UC__n_78, __UC__n_79,
         __UC__n_80, __UC__n_81,
         __UC__n_82, __UC__n_83,
         __UC__n_84, __UC__n_85,
         __UC__n_86, __UC__n_87,
         __UC__n_88, __UC__n_89,
         __UC__n_90, __UC__n_91,
         __UC__n_92, __UC__n_93,
         __UC__n_94, __UC__n_95,
         __UC__n_96, __UC__n_97,
         __UC__n_98, __UC__n_99,
         __UC__n_100, __UC__n_101,
         __UC__n_102, __UC__n_103,
         __UC__n_104, __UC__n_105,
         __UC__n_106, __UC__n_107,
         __UC__n_108, __UC__n_109,
         __UC__n_110, __UC__n_111,
         __UC__n_112, __UC__n_113,
         __UC__n_114, __UC__n_115,
         __UC__n_116, __UC__n_117,
         __UC__n_118, __UC__n_119,
         __UC__n_120, __UC__n_121,
         __UC__n_122, __UC__n_123,
         __UC__n_124, __UC__n_125,
         __UC__n_126, __UC__n_127,
         __UC__n_128, __UC__n_129,
         __UC__n_130, __UC__n_131,
         __UC__n_132, __UC__n_133,
         __UC__n_134, __UC__n_135,
         __UC__n_136, __UC__n_137,
         __UC__n_138, __UC__n_139,
         __UC__n_140, __UC__n_141,
         __UC__n_142, __UC__n_143,
         __UC__n_144, __UC__n_145,
         __UC__n_146, __UC__n_147,
         __UC__n_148, __UC__n_149,
         __UC__n_150, __UC__n_151,
         __UC__n_152, __UC__n_153,
         __UC__n_154, __UC__n_155,
         __UC__n_156, __UC__n_157,
         __UC__n_158, __UC__n_159,
         __UC__n_160, __UC__n_161,
         __UC__n_162, __UC__n_163,
         __UC__n_164, __UC__n_165,
         __UC__n_166, __UC__n_167,
         __UC__n_168, __UC__n_169;
  wire   [127:0] biu_ifu_rdata;
  wire   [1:0] biu_ifu_rresp;
  wire   [127:0] biu_lsu_rdata;
  wire   [3:0] biu_lsu_rid;
  wire   [1:0] biu_lsu_rresp;
  wire   [63:0] clint_core0_time;
  wire   [1:0] core0_sysio_lpmd_b;
  wire   [63:0] dtu_tdt_dm_rx_data;
  wire   [39:4] ifu_biu_araddr;
  wire   [1:0] ifu_biu_arburst;
  wire   [3:0] ifu_biu_arcache;
  wire   [1:0] ifu_biu_arlen;
  wire   [39:0] lsu_biu_araddr;
  wire   [1:0] lsu_biu_arburst;
  wire   [3:0] lsu_biu_arcache;
  wire   [3:0] lsu_biu_arid;
  wire   [1:0] lsu_biu_arlen;
  wire   [2:0] lsu_biu_arsize;
  wire   [39:0] lsu_biu_stb_awaddr;
  wire   [3:0] lsu_biu_stb_awcache;
  wire   [1:0] lsu_biu_stb_awlen;
  wire   [2:0] lsu_biu_stb_awprot;
  wire   [2:0] lsu_biu_stb_awsize;
  wire   [127:0] lsu_biu_stb_wdata;
  wire   [15:0] lsu_biu_stb_wstrb;
  wire   [39:6] lsu_biu_vb_awaddr;
  wire   [127:0] lsu_biu_vb_wdata;
  wire   [39:27] sysio_cp0_apb_base;
  wire   [39:0] sysio_xx_rvba;
  wire   [31:0] tdt_dm_dtu_itr;
  wire   [63:0] tdt_dm_dtu_wdata;
  wire   [1:0] tdt_dm_dtu_wr_flg;
  wire   [31:0] paddr;
  wire   [1:0] pprot;
  wire   [31:0] prdata_clint;
  wire   [31:0] prdata_plic;
  wire   [31:0] pwdata;
  wire   [39:27] sysio_biu_apb_base;
  wire   [63:0] sysio_clint_mtime;
  wire   pll_core_cpuclk;
  wire   forever_cpuclk;
  assign biu_pad_arid[7] = tdt_dm_pad_arlen[0];
  assign biu_pad_arid[6] = tdt_dm_pad_arlen[0];
  assign biu_pad_arid[5] = tdt_dm_pad_arlen[0];
  assign biu_pad_arid[4] = tdt_dm_pad_arlen[0];
  assign biu_pad_arlen[7] = tdt_dm_pad_arlen[0];
  assign biu_pad_arlen[6] = tdt_dm_pad_arlen[0];
  assign biu_pad_arlen[5] = tdt_dm_pad_arlen[0];
  assign biu_pad_arlen[4] = tdt_dm_pad_arlen[0];
  assign biu_pad_arlen[3] = tdt_dm_pad_arlen[0];
  assign biu_pad_arlen[2] = tdt_dm_pad_arlen[0];
  assign biu_pad_awid[7] = tdt_dm_pad_arlen[0];
  assign biu_pad_awid[6] = tdt_dm_pad_arlen[0];
  assign biu_pad_awid[5] = tdt_dm_pad_arlen[0];
  assign biu_pad_awid[4] = tdt_dm_pad_arlen[0];
  assign biu_pad_awlen[7] = tdt_dm_pad_arlen[0];
  assign biu_pad_awlen[6] = tdt_dm_pad_arlen[0];
  assign biu_pad_awlen[5] = tdt_dm_pad_arlen[0];
  assign biu_pad_awlen[4] = tdt_dm_pad_arlen[0];
  assign biu_pad_awlen[3] = tdt_dm_pad_arlen[0];
  assign biu_pad_awlen[2] = tdt_dm_pad_arlen[0];
  assign tdt_dmi_pslverr = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_awburst[1] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_awcache[3] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_awcache[2] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_awcache[1] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_awcache[0] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_awlock = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_awprot[2] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_awprot[0] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_arburst[1] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_arcache[3] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_arcache[2] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_arcache[1] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_arcache[0] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_arlock = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_arprot[2] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_arprot[0] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_awid[3] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_awid[2] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_awid[1] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_awid[0] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_awlen[3] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_awlen[2] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_awlen[1] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_awlen[0] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_arid[3] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_arid[2] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_arid[1] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_arid[0] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_arlen[3] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_arlen[2] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_arlen[1] = tdt_dm_pad_arlen[0];
  assign tdt_dm_pad_rready = biu_pad_bready;
  assign tdt_dm_pad_bready = biu_pad_bready;
  assign tdt_dm_pad_wlast = biu_pad_bready;
  assign tdt_dm_pad_arprot[1] = biu_pad_bready;
  assign tdt_dm_pad_arburst[0] = biu_pad_bready;
  assign tdt_dm_pad_awprot[1] = biu_pad_bready;
  assign tdt_dm_pad_awburst[0] = biu_pad_bready;
  assign biu_pad_rready = biu_pad_bready;

  aq_top_0 x_aq_top_0 ( .biu_ifu_arready(biu_ifu_arready), .biu_ifu_rdata(
        biu_ifu_rdata), .biu_ifu_rid(biu_ifu_rid), .biu_ifu_rlast(
        biu_ifu_rlast), .biu_ifu_rresp(biu_ifu_rresp), .biu_ifu_rvalid(
        biu_ifu_rvalid), .biu_lsu_arready(biu_lsu_arready), .biu_lsu_no_op(
        biu_lsu_no_op), .biu_lsu_rdata(biu_lsu_rdata), .biu_lsu_rid(
        biu_lsu_rid), .biu_lsu_rlast(biu_lsu_rlast), .biu_lsu_rresp(
        biu_lsu_rresp), .biu_lsu_rvalid(biu_lsu_rvalid), .biu_lsu_stb_awready(
        biu_lsu_stb_awready), .biu_lsu_stb_wready(biu_lsu_stb_wready), 
        .biu_lsu_vb_awready(biu_lsu_vb_awready), .biu_lsu_vb_wready(
        biu_lsu_vb_wready), .clint_cpuio_time(clint_core0_time), 
        .cp0_biu_icg_en(cp0_biu_icg_en), .cpuio_sysio_lpmd_b(
        core0_sysio_lpmd_b), .cpurst_b(core0_rst_b), .dtu_tdt_dm_halted(
        dtu_tdt_dm_halted), .dtu_tdt_dm_havereset(dtu_tdt_dm_havereset), 
        .dtu_tdt_dm_itr_done(dtu_tdt_dm_itr_done), 
        .dtu_tdt_dm_retire_debug_expt_vld(dtu_tdt_dm_retire_debug_expt_vld), 
        .dtu_tdt_dm_rx_data(dtu_tdt_dm_rx_data), .dtu_tdt_dm_wr_ready(
        dtu_tdt_dm_wr_ready), .forever_cpuclk(forever_cpuclk), 
        .ifu_biu_araddr({ifu_biu_araddr, __UC__n_1, 
        __UC__n_2, __UC__n_3, __UC__n_4}), .ifu_biu_arburst(ifu_biu_arburst), .ifu_biu_arcache({ifu_biu_arcache[3:2], 
        __UC__n_5, ifu_biu_arcache[0]}), .ifu_biu_arid(
        ifu_biu_arid), .ifu_biu_arlen(ifu_biu_arlen), .ifu_biu_arprot({
        __UC__n_6, __UC__n_7, ifu_biu_arprot_0_}), 
        .ifu_biu_arsize({__UC__n_8, __UC__n_9, 
        __UC__n_10}), .ifu_biu_arvalid(ifu_biu_arvalid), 
        .lsu_biu_araddr(lsu_biu_araddr), .lsu_biu_arburst(lsu_biu_arburst), 
        .lsu_biu_arcache(lsu_biu_arcache), .lsu_biu_arid(lsu_biu_arid), 
        .lsu_biu_arlen(lsu_biu_arlen), .lsu_biu_arprot({
        __UC__n_11, __UC__n_12, lsu_biu_arprot_0_}), 
        .lsu_biu_arsize(lsu_biu_arsize), .lsu_biu_aruser(lsu_biu_aruser), 
        .lsu_biu_arvalid(lsu_biu_arvalid), .lsu_biu_stb_awaddr(
        lsu_biu_stb_awaddr), .lsu_biu_stb_awburst({__UC__n_13, 
        __UC__n_14}), .lsu_biu_stb_awcache(lsu_biu_stb_awcache), 
        .lsu_biu_stb_awid({__UC__n_15, __UC__n_16}), 
        .lsu_biu_stb_awlen(lsu_biu_stb_awlen), .lsu_biu_stb_awprot(
        lsu_biu_stb_awprot), .lsu_biu_stb_awsize(lsu_biu_stb_awsize), 
        .lsu_biu_stb_awuser(lsu_biu_stb_awuser), .lsu_biu_stb_awvalid(
        lsu_biu_stb_awvalid), .lsu_biu_stb_wdata(lsu_biu_stb_wdata), 
        .lsu_biu_stb_wlast(lsu_biu_stb_wlast), .lsu_biu_stb_wstrb(
        lsu_biu_stb_wstrb), .lsu_biu_stb_wvalid(lsu_biu_stb_wvalid), 
        .lsu_biu_vb_awaddr({lsu_biu_vb_awaddr, __UC__n_17, 
        __UC__n_18, __UC__n_19, 
        __UC__n_20, __UC__n_21, 
        __UC__n_22}), .lsu_biu_vb_awburst({
        __UC__n_23, __UC__n_24}), 
        .lsu_biu_vb_awcache({__UC__n_25, __UC__n_26, 
        __UC__n_27, __UC__n_28}), .lsu_biu_vb_awid({
        __UC__n_29, __UC__n_30, 
        __UC__n_31, __UC__n_32}), .lsu_biu_vb_awlen(
        {__UC__n_33, __UC__n_34}), 
        .lsu_biu_vb_awprot({__UC__n_35, __UC__n_36, 
        __UC__n_37}), .lsu_biu_vb_awsize({__UC__n_38, 
        __UC__n_39, __UC__n_40}), 
        .lsu_biu_vb_awvalid(lsu_biu_vb_awvalid), .lsu_biu_vb_wdata(
        lsu_biu_vb_wdata), .lsu_biu_vb_wlast(lsu_biu_vb_wlast), 
        .lsu_biu_vb_wstrb({__UC__n_41, __UC__n_42, 
        __UC__n_43, __UC__n_44, 
        __UC__n_45, __UC__n_46, 
        __UC__n_47, __UC__n_48, 
        __UC__n_49, __UC__n_50, 
        __UC__n_51, __UC__n_52, 
        __UC__n_53, __UC__n_54, 
        __UC__n_55, __UC__n_56}), 
        .lsu_biu_vb_wvalid(lsu_biu_vb_wvalid), .pad_biu_coreid({
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0]}), 
        .pad_yy_icg_scan_en(pad_yy_icg_scan_en), .pad_yy_scan_mode(
        pad_yy_scan_mode), .rtu_cpu_no_retire(cpu_debug_port), 
        .rtu_pad_halted(core0_pad_halted), .rtu_pad_retire(core0_pad_retire), 
        .rtu_pad_retire_pc(core0_pad_retire_pc), .sys_apb_clk(sys_apb_clk), 
        .sys_apb_rst_b(sync_sys_apb_rst_b), .sysio_cp0_apb_base({
        sysio_cp0_apb_base, tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0]}), .sysio_cpuio_me_int(sysio_core0_me_int), 
        .sysio_cpuio_ms_int(sysio_core0_ms_int), .sysio_cpuio_mt_int(
        sysio_core0_mt_int), .sysio_cpuio_se_int(sysio_core0_se_int), 
        .sysio_cpuio_ss_int(sysio_core0_ss_int), .sysio_cpuio_st_int(
        sysio_core0_st_int), .sysio_xx_rvba(sysio_xx_rvba), 
        .tdt_dm_dtu_ack_havereset(tdt_dm_dtu_ack_havereset), 
        .tdt_dm_dtu_async_halt_req(tdt_dm_dtu_async_halt_req), 
        .tdt_dm_dtu_halt_on_reset(tdt_dm_dtu_halt_on_reset), 
        .tdt_dm_dtu_halt_req(tdt_dm_dtu_halt_req), .tdt_dm_dtu_itr(
        tdt_dm_dtu_itr), .tdt_dm_dtu_itr_vld(tdt_dm_dtu_itr_vld), 
        .tdt_dm_dtu_resume_req(tdt_dm_dtu_resume_req), .tdt_dm_dtu_wdata(
        tdt_dm_dtu_wdata), .tdt_dm_dtu_wr_flg(tdt_dm_dtu_wr_flg), 
        .tdt_dm_dtu_wr_vld(tdt_dm_dtu_wr_vld) );
  aq_biu_top_0 x_aq_biu_top ( .apb_clk_en(apb_clk_en), .axim_clk_en(
        axim_clk_en_f), .biu_ifu_arready(biu_ifu_arready), .biu_ifu_rdata(
        biu_ifu_rdata), .biu_ifu_rid(biu_ifu_rid), .biu_ifu_rlast(
        biu_ifu_rlast), .biu_ifu_rresp(biu_ifu_rresp), .biu_ifu_rvalid(
        biu_ifu_rvalid), .biu_lsu_arready(biu_lsu_arready), .biu_lsu_no_op(
        biu_lsu_no_op), .biu_lsu_rdata(biu_lsu_rdata), .biu_lsu_rid(
        biu_lsu_rid), .biu_lsu_rlast(biu_lsu_rlast), .biu_lsu_rresp(
        biu_lsu_rresp), .biu_lsu_rvalid(biu_lsu_rvalid), .biu_lsu_stb_awready(
        biu_lsu_stb_awready), .biu_lsu_stb_wready(biu_lsu_stb_wready), 
        .biu_lsu_vb_awready(biu_lsu_vb_awready), .biu_lsu_vb_wready(
        biu_lsu_vb_wready), .biu_pad_araddr(biu_pad_araddr), .biu_pad_arburst(
        biu_pad_arburst), .biu_pad_arcache(biu_pad_arcache), .biu_pad_arid({
        __UC__n_57, __UC__n_58, 
        __UC__n_59, __UC__n_60, biu_pad_arid[3:0]}), 
        .biu_pad_arlen({__UC__n_61, __UC__n_62, 
        __UC__n_63, __UC__n_64, 
        __UC__n_65, __UC__n_66, biu_pad_arlen[1:0]}), 
        .biu_pad_arlock(biu_pad_arlock), .biu_pad_arprot(biu_pad_arprot), 
        .biu_pad_arsize(biu_pad_arsize), .biu_pad_arvalid(biu_pad_arvalid), 
        .biu_pad_awaddr(biu_pad_awaddr), .biu_pad_awburst(biu_pad_awburst), 
        .biu_pad_awcache(biu_pad_awcache), .biu_pad_awid({
        __UC__n_67, __UC__n_68, 
        __UC__n_69, __UC__n_70, biu_pad_awid[3:0]}), 
        .biu_pad_awlen({__UC__n_71, __UC__n_72, 
        __UC__n_73, __UC__n_74, 
        __UC__n_75, __UC__n_76, biu_pad_awlen[1:0]}), 
        .biu_pad_awlock(biu_pad_awlock), .biu_pad_awprot(biu_pad_awprot), 
        .biu_pad_awsize(biu_pad_awsize), .biu_pad_awvalid(biu_pad_awvalid), 
        .biu_pad_wdata(biu_pad_wdata), .biu_pad_wlast(biu_pad_wlast), 
        .biu_pad_wstrb(biu_pad_wstrb), .biu_pad_wvalid(biu_pad_wvalid), 
        .cp0_biu_icg_en(cp0_biu_icg_en), .cpurst_b(n2), .forever_cpuclk(
        forever_cpuclk), .ifu_biu_araddr({ifu_biu_araddr, tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0]}), 
        .ifu_biu_arburst(ifu_biu_arburst), .ifu_biu_arcache({
        ifu_biu_arcache[3:2], biu_pad_bready, ifu_biu_arcache[0]}), 
        .ifu_biu_arid(ifu_biu_arid), .ifu_biu_arlen(ifu_biu_arlen), 
        .ifu_biu_arprot({biu_pad_bready, biu_pad_bready, ifu_biu_arprot_0_}), 
        .ifu_biu_arsize({biu_pad_bready, tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0]}), .ifu_biu_arvalid(ifu_biu_arvalid), 
        .lsu_biu_araddr(lsu_biu_araddr), .lsu_biu_arburst(lsu_biu_arburst), 
        .lsu_biu_arcache(lsu_biu_arcache), .lsu_biu_arid(lsu_biu_arid), 
        .lsu_biu_arlen(lsu_biu_arlen), .lsu_biu_arprot({tdt_dm_pad_arlen[0], 
        biu_pad_bready, lsu_biu_arprot_0_}), .lsu_biu_arsize(lsu_biu_arsize), 
        .lsu_biu_aruser(lsu_biu_aruser), .lsu_biu_arvalid(lsu_biu_arvalid), 
        .lsu_biu_stb_awaddr(lsu_biu_stb_awaddr), .lsu_biu_stb_awburst({
        tdt_dm_pad_arlen[0], biu_pad_bready}), .lsu_biu_stb_awcache(
        lsu_biu_stb_awcache), .lsu_biu_stb_awid({tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0]}), .lsu_biu_stb_awlen(lsu_biu_stb_awlen), 
        .lsu_biu_stb_awprot(lsu_biu_stb_awprot), .lsu_biu_stb_awsize(
        lsu_biu_stb_awsize), .lsu_biu_stb_awuser(lsu_biu_stb_awuser), 
        .lsu_biu_stb_awvalid(lsu_biu_stb_awvalid), .lsu_biu_stb_wdata(
        lsu_biu_stb_wdata), .lsu_biu_stb_wlast(lsu_biu_stb_wlast), 
        .lsu_biu_stb_wstrb(lsu_biu_stb_wstrb), .lsu_biu_stb_wvalid(
        lsu_biu_stb_wvalid), .lsu_biu_vb_awaddr({lsu_biu_vb_awaddr, 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0]}), 
        .lsu_biu_vb_awburst({tdt_dm_pad_arlen[0], biu_pad_bready}), 
        .lsu_biu_vb_awcache({biu_pad_bready, biu_pad_bready, biu_pad_bready, 
        biu_pad_bready}), .lsu_biu_vb_awid({tdt_dm_pad_arlen[0], 
        biu_pad_bready, tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0]}), 
        .lsu_biu_vb_awlen({biu_pad_bready, biu_pad_bready}), 
        .lsu_biu_vb_awprot({tdt_dm_pad_arlen[0], biu_pad_bready, 
        biu_pad_bready}), .lsu_biu_vb_awsize({biu_pad_bready, 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0]}), .lsu_biu_vb_awvalid(
        lsu_biu_vb_awvalid), .lsu_biu_vb_wdata(lsu_biu_vb_wdata), 
        .lsu_biu_vb_wlast(lsu_biu_vb_wlast), .lsu_biu_vb_wstrb({biu_pad_bready, 
        biu_pad_bready, biu_pad_bready, biu_pad_bready, biu_pad_bready, 
        biu_pad_bready, biu_pad_bready, biu_pad_bready, biu_pad_bready, 
        biu_pad_bready, biu_pad_bready, biu_pad_bready, biu_pad_bready, 
        biu_pad_bready, biu_pad_bready, biu_pad_bready}), .lsu_biu_vb_wvalid(
        lsu_biu_vb_wvalid), .pad_biu_arready(pad_biu_arready), 
        .pad_biu_awready(pad_biu_awready), .pad_biu_bid(pad_biu_bid), 
        .pad_biu_bresp(pad_biu_bresp), .pad_biu_bvalid(pad_biu_bvalid), 
        .pad_biu_rdata(pad_biu_rdata), .pad_biu_rid(pad_biu_rid), 
        .pad_biu_rlast(pad_biu_rlast), .pad_biu_rresp(pad_biu_rresp), 
        .pad_biu_rvalid(pad_biu_rvalid), .pad_biu_wready(pad_biu_wready), 
        .pad_yy_icg_scan_en(pad_yy_icg_scan_en), .paddr(paddr), .penable(
        penable), .perr_clint(perr_clint), .perr_plic(perr_plic), .pprot(pprot), .prdata_clint(prdata_clint), .prdata_plic(prdata_plic), .pready_clint(
        pready_clint), .pready_plic(pready_plic), .psel_clint(psel_clint), 
        .psel_plic(psel_plic), .pwdata(pwdata), .pwrite(pwrite), 
        .sysio_biu_apb_base({sysio_biu_apb_base, tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0]}) );
  aq_mp_rst_top_0 x_aq_mp_rst_top ( .ciu_rst_b(ciu_rst_b), .clkgen_rst_b(
        clkgen_rst_b), .core0_rst_b(core0_rst_b), .forever_cpuclk(
        forever_cpuclk), .pad_cpu_rst_b(pad_cpu_rst_b), .pad_yy_dft_clk_rst_b(
        pad_yy_dft_clk_rst_b), .pad_yy_mbist_mode(pad_yy_mbist_mode), 
        .pad_yy_scan_mode(pad_yy_scan_mode), .pad_yy_scan_rst_b(
        pad_yy_scan_rst_b), .sync_sys_apb_rst_b(sync_sys_apb_rst_b), 
        .sys_apb_clk(sys_apb_clk), .sys_apb_rst_b(sys_apb_rst_b) );
  aq_mp_clk_top_0 x_aq_mp_clk_top ( .apb_clk(apb_clk), .apb_clk_en(apb_clk_en), 
        .axim_clk_en(axim_clk_en), .axim_clk_en_f(axim_clk_en_f), 
        .clkgen_rst_b(clkgen_rst_b), .forever_cpuclk(forever_cpuclk), 
        .pad_yy_scan_mode(pad_yy_scan_mode), .pll_core_cpuclk(pll_core_cpuclk)
         );
  aq_sysio_top_0 x_aq_sysio_top ( .apb_clk_en(apb_clk_en), .axim_clk_en(
        axim_clk_en_f), .clint_core0_ms_int(clint_core0_ms_int), 
        .clint_core0_mt_int(clint_core0_mt_int), .clint_core0_ss_int(
        clint_core0_ss_int), .clint_core0_st_int(clint_core0_st_int), 
        .core0_pad_lpmd_b(core0_pad_lpmd_b), .core0_sysio_lpmd_b(
        core0_sysio_lpmd_b), .cpurst_b(n2), .forever_cpuclk(forever_cpuclk), 
        .pad_cpu_apb_base(pad_cpu_apb_base), .pad_cpu_rvba(pad_cpu_rvba), 
        .pad_cpu_sys_cnt(pad_cpu_sys_cnt), .pad_yy_icg_scan_en(
        pad_yy_icg_scan_en), .plic_core0_me_int(plic_hartx_mint_req_0_), 
        .plic_core0_se_int(plic_hartx_sint_req_0_), .sysio_biu_apb_base({
        sysio_biu_apb_base, __UC__n_77, __UC__n_78, 
        __UC__n_79, __UC__n_80, 
        __UC__n_81, __UC__n_82, 
        __UC__n_83, __UC__n_84, 
        __UC__n_85, __UC__n_86, 
        __UC__n_87, __UC__n_88, 
        __UC__n_89, __UC__n_90, 
        __UC__n_91, __UC__n_92, 
        __UC__n_93, __UC__n_94, 
        __UC__n_95, __UC__n_96, 
        __UC__n_97, __UC__n_98, 
        __UC__n_99, __UC__n_100, 
        __UC__n_101, __UC__n_102, 
        __UC__n_103}), .sysio_clint_mtime(sysio_clint_mtime), 
        .sysio_core0_hartid({__UC__n_104, 
        __UC__n_105, __UC__n_106}), 
        .sysio_core0_me_int(sysio_core0_me_int), .sysio_core0_ms_int(
        sysio_core0_ms_int), .sysio_core0_mt_int(sysio_core0_mt_int), 
        .sysio_core0_se_int(sysio_core0_se_int), .sysio_core0_ss_int(
        sysio_core0_ss_int), .sysio_core0_st_int(sysio_core0_st_int), 
        .sysio_cp0_apb_base({sysio_cp0_apb_base, __UC__n_107, 
        __UC__n_108, __UC__n_109, 
        __UC__n_110, __UC__n_111, 
        __UC__n_112, __UC__n_113, 
        __UC__n_114, __UC__n_115, 
        __UC__n_116, __UC__n_117, 
        __UC__n_118, __UC__n_119, 
        __UC__n_120, __UC__n_121, 
        __UC__n_122, __UC__n_123, 
        __UC__n_124, __UC__n_125, 
        __UC__n_126, __UC__n_127, 
        __UC__n_128, __UC__n_129, 
        __UC__n_130, __UC__n_131, 
        __UC__n_132, __UC__n_133}), .sysio_xx_rvba(
        sysio_xx_rvba) );
  tdt_top_0 x_tdt_top ( .ciu_rst_b(n2), .dtu_tdt_dm_halted(dtu_tdt_dm_halted), 
        .dtu_tdt_dm_havereset(dtu_tdt_dm_havereset), .dtu_tdt_dm_itr_done(
        dtu_tdt_dm_itr_done), .dtu_tdt_dm_retire_debug_expt_vld(
        dtu_tdt_dm_retire_debug_expt_vld), .dtu_tdt_dm_rx_data(
        dtu_tdt_dm_rx_data), .dtu_tdt_dm_wr_ready(dtu_tdt_dm_wr_ready), 
        .forever_cpuclk(forever_cpuclk), .pad_tdt_dm_arready(
        pad_tdt_dm_arready), .pad_tdt_dm_awready(pad_tdt_dm_awready), 
        .pad_tdt_dm_bid(pad_tdt_dm_bid), .pad_tdt_dm_bresp(pad_tdt_dm_bresp), 
        .pad_tdt_dm_bvalid(pad_tdt_dm_bvalid), .pad_tdt_dm_core_unavail(
        pad_tdt_dm_core_unavail), .pad_tdt_dm_rdata(pad_tdt_dm_rdata), 
        .pad_tdt_dm_rid(pad_tdt_dm_rid), .pad_tdt_dm_rlast(pad_tdt_dm_rlast), 
        .pad_tdt_dm_rresp(pad_tdt_dm_rresp), .pad_tdt_dm_rvalid(
        pad_tdt_dm_rvalid), .pad_tdt_dm_wready(pad_tdt_dm_wready), 
        .pad_yy_icg_scan_en(pad_yy_icg_scan_en), .pad_yy_scan_mode(
        pad_yy_scan_mode), .pad_yy_scan_rst_b(pad_yy_scan_rst_b), 
        .sys_apb_clk(sys_apb_clk), .sys_apb_rst_b(sync_sys_apb_rst_b), 
        .sys_bus_clk_en(axim_clk_en_f), .tdt_dm_dtu_ack_havereset(
        tdt_dm_dtu_ack_havereset), .tdt_dm_dtu_async_halt_req(
        tdt_dm_dtu_async_halt_req), .tdt_dm_dtu_halt_on_reset(
        tdt_dm_dtu_halt_on_reset), .tdt_dm_dtu_halt_req(tdt_dm_dtu_halt_req), 
        .tdt_dm_dtu_halt_req_cause({__UC__n_134, 
        __UC__n_135}), .tdt_dm_dtu_itr(tdt_dm_dtu_itr), 
        .tdt_dm_dtu_itr_vld(tdt_dm_dtu_itr_vld), .tdt_dm_dtu_resume_req(
        tdt_dm_dtu_resume_req), .tdt_dm_dtu_wdata(tdt_dm_dtu_wdata), 
        .tdt_dm_dtu_wr_flg(tdt_dm_dtu_wr_flg), .tdt_dm_dtu_wr_vld(
        tdt_dm_dtu_wr_vld), .tdt_dm_pad_araddr(tdt_dm_pad_araddr), 
        .tdt_dm_pad_arburst({__UC__n_136, 
        __UC__n_137}), .tdt_dm_pad_arcache({
        __UC__n_138, __UC__n_139, 
        __UC__n_140, __UC__n_141}), 
        .tdt_dm_pad_arid({__UC__n_142, __UC__n_143, 
        __UC__n_144, __UC__n_145}), 
        .tdt_dm_pad_arlen({__UC__n_146, __UC__n_147, 
        __UC__n_148, __UC__n_149}), 
        .tdt_dm_pad_arprot({__UC__n_150, __UC__n_151, 
        __UC__n_152}), .tdt_dm_pad_arsize(tdt_dm_pad_arsize), 
        .tdt_dm_pad_arvalid(tdt_dm_pad_arvalid), .tdt_dm_pad_awaddr(
        tdt_dm_pad_awaddr), .tdt_dm_pad_awburst({__UC__n_153, 
        __UC__n_154}), .tdt_dm_pad_awcache({
        __UC__n_155, __UC__n_156, 
        __UC__n_157, __UC__n_158}), 
        .tdt_dm_pad_awid({__UC__n_159, __UC__n_160, 
        __UC__n_161, __UC__n_162}), 
        .tdt_dm_pad_awlen({__UC__n_163, __UC__n_164, 
        __UC__n_165, __UC__n_166}), 
        .tdt_dm_pad_awprot({__UC__n_167, __UC__n_168, 
        __UC__n_169}), .tdt_dm_pad_awsize(tdt_dm_pad_awsize), 
        .tdt_dm_pad_awvalid(tdt_dm_pad_awvalid), .tdt_dm_pad_hartreset_n(
        tdt_dm_pad_hartreset_n), .tdt_dm_pad_ndmreset_n(tdt_dm_pad_ndmreset_n), 
        .tdt_dm_pad_wdata(tdt_dm_pad_wdata), .tdt_dm_pad_wstrb(
        tdt_dm_pad_wstrb), .tdt_dm_pad_wvalid(tdt_dm_pad_wvalid), 
        .tdt_dmi_paddr(tdt_dmi_paddr), .tdt_dmi_penable(tdt_dmi_penable), 
        .tdt_dmi_prdata(tdt_dmi_prdata), .tdt_dmi_pready(tdt_dmi_pready), 
        .tdt_dmi_psel(tdt_dmi_psel), .tdt_dmi_pwdata(tdt_dmi_pwdata), 
        .tdt_dmi_pwrite(tdt_dmi_pwrite) );
  clint_top_0 x_clint_top ( .apb_clk_en(apb_clk_en), .ciu_clk(forever_cpuclk), 
        .clint_core0_ms_int(clint_core0_ms_int), .clint_core0_mt_int(
        clint_core0_mt_int), .clint_core0_ss_int(clint_core0_ss_int), 
        .clint_core0_st_int(clint_core0_st_int), .clint_core0_time(
        clint_core0_time), .cpurst_b(n2), .forever_apbclk(apb_clk), 
        .pad_yy_icg_scan_en(pad_yy_icg_scan_en), .paddr(paddr), .penable(
        penable), .perr_clint(perr_clint), .pprot(pprot), .prdata_clint(
        prdata_clint), .pready_clint(pready_clint), .psel_clint(psel_clint), 
        .pwdata(pwdata), .pwrite(pwrite), .sysio_clint_mtime(sysio_clint_mtime) );
  plic_top_256_10_01_5_32_0 x_aq_plic_top ( .plic_hartx_mint_req(
        plic_hartx_mint_req_0_), .plic_hartx_sint_req(plic_hartx_sint_req_0_), 
        .ciu_plic_paddr(paddr[26:0]), .ciu_plic_penable(penable), 
        .ciu_plic_psel(psel_plic), .ciu_plic_pprot(pprot), .ciu_plic_pwdata(
        pwdata), .ciu_plic_pwrite(pwrite), .ciu_plic_icg_en(
        tdt_dm_pad_arlen[0]), .pad_plic_int_vld({pad_plic_int_vld, 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0]}), .pad_plic_int_cfg({pad_plic_int_cfg, 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], tdt_dm_pad_arlen[0], 
        tdt_dm_pad_arlen[0]}), .pad_yy_icg_scan_en(pad_yy_icg_scan_en), 
        .plic_ciu_prdata(prdata_plic), .plic_ciu_pready(pready_plic), 
        .plic_ciu_pslverr(perr_plic), .plic_clk(apb_clk), .plicrst_b(n2) );
  sky130_fd_sc_hd__inv_2 U2 ( .A(n3), .Y(n2) );
  sky130_fd_sc_hd__inv_2 U3 ( .A(ciu_rst_b), .Y(n3) );
  sky130_fd_sc_hd__conb_1 U4 ( .LO(tdt_dm_pad_arlen[0]), .HI(biu_pad_bready)
         );
endmodule

