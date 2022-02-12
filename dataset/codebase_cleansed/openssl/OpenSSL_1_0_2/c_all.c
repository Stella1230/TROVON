void OPENSSL_add_all_algorithms_noconf(void)
{
OPENSSL_cpuid_setup();
OpenSSL_add_all_ciphers();
OpenSSL_add_all_digests();
#ifndef OPENSSL_NO_ENGINE
# if defined(__OpenBSD__) || defined(__FreeBSD__) || defined(HAVE_CRYPTODEV)
ENGINE_setup_bsd_cryptodev();
# endif
#endif
}
