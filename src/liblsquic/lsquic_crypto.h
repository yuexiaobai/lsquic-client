/* Copyright (c) 2017 - 2018 LiteSpeed Technologies Inc.  See LICENSE. */

#ifndef __LSQUIC_CRYPTO_H__
#define __LSQUIC_CRYPTO_H__

#include <stdint.h>

#define HS_PKT_HASH_LENGTH 12


#ifdef __cplusplus
extern "C" {
#endif

struct lsquic_str;

#if defined( __x86_64 )||defined( __x86_64__ )
    typedef __uint128_t uint128;
#else
    typedef struct uint128_st
    {
        uint64_t hi_;
        uint64_t lo_;
    } uint128;
#endif


void crypto_init(void);

/* XXX: why have a wrapper around RAND_bytes? */
void rand_bytes(void *data, int len);


int export_key_material_simple(unsigned char *ikm, uint32_t ikm_len,
                        unsigned char *salt, int salt_len,
                        char *label, uint32_t label_len,
                        const uint8_t *context, uint32_t context_len,
                        uint8_t *key, uint16_t key_len);

int export_key_material(const unsigned char *ikm, uint32_t ikm_len,
                        const unsigned char *salt, int salt_len,
                        const unsigned char *context, uint32_t context_len,
                        uint16_t c_key_len, uint8_t *c_key,
                        uint16_t s_key_len, uint8_t *s_key,
                        uint16_t c_key_iv_len, uint8_t *c_key_iv,
                        uint16_t s_key_iv_len, uint8_t *s_key_iv,
                        uint8_t *sub_key);

void c255_get_pub_key(unsigned char *priv_key, unsigned char pub_key[32]);
int c255_gen_share_key(unsigned char *priv_key, unsigned char *peer_pub_key, unsigned char *shared_key);



uint64_t fnv1a_64(const uint8_t * data, int len);
void fnv1a_64_s(const uint8_t * data, int len, char *md);
uint128 fnv1a_128(const uint8_t * data, int len);
void fnv1a_128_s(const uint8_t * data , int len, uint8_t  *md);
uint128 fnv1a_128_2(const uint8_t * data1, int len1, const uint8_t * data2, int len2);
uint128 fnv1a_128_3(const uint8_t * data1, int len1,
                      const uint8_t * data2, int len2,
                      const uint8_t * data3, int len3);
void fnv1a_128_2_s(const uint8_t * data1, int len1, const uint8_t * data2, int len2, uint8_t  *md);
void serialize_fnv128_short(uint128 v, uint8_t *md);


/* before session handshake complete */
int verify_hs_pkt(const uint8_t *pkg_data, size_t header_len, size_t pkg_len);
int update_hs_pkt_hash(uint8_t *pkg_data, int header_len, int pkg_len);
int get_hs_pkt_hash_len();


/*16 bytes of h outputted  */
void sha256(const uint8_t *buf, int len, uint8_t *h);


/* Encrypt plaint text to cipher test */
int aes_aead_enc(EVP_AEAD_CTX *key,
              const uint8_t *ad, size_t ad_len,
              const uint8_t *nonce, size_t nonce_len, 
              const uint8_t *plain, size_t plain_len,
              uint8_t *cypher, size_t *cypher_len);

int aes_aead_dec(EVP_AEAD_CTX *key,
              const uint8_t *ad, size_t ad_len,
              const uint8_t *nonce, size_t nonce_len, 
              const uint8_t *cypher, size_t cypher_len,
              uint8_t *plain, size_t *plain_len);

int aes_get_key_length();

void gen_nonce_s(char *buf, int length);

/* 32 bytes client nonce with 4 bytes tm, 8 bytes orbit */
void gen_nonce_c(unsigned char *buf, uint64_t orbit);

EVP_PKEY *PEM_to_key(const char *buf, int len);

X509 *bio_to_crt(const void *buf, int len, int type);

int lshkdf_expand(const unsigned char *prk, const unsigned char *info, int info_len,
                uint16_t c_key_len, uint8_t *c_key,
                uint16_t s_key_len, uint8_t *s_key,
                uint16_t c_key_iv_len, uint8_t *c_key_iv,
                uint16_t s_key_iv_len, uint8_t *s_key_iv,
                uint16_t sub_key_len, uint8_t *sub_key);
void lshkdf_extract(const unsigned char *ikm, int ikm_len, const unsigned char *salt,
                  int salt_len, unsigned char *prk);

int gen_prof(const uint8_t *chlo_data, size_t chlo_data_len,
             const uint8_t *scfg_data, uint32_t scfg_data_len,
             const EVP_PKEY *priv_key, uint8_t *buf, size_t *len);
int verify_prof0(const uint8_t *chlo_data, size_t chlo_data_len,
                const uint8_t *scfg_data, uint32_t scfg_data_len,
                const EVP_PKEY *pub_key, const uint8_t *buf, size_t len);

int verify_prof(const uint8_t *chlo_data, size_t chlo_data_len, struct lsquic_str * scfg,
                const EVP_PKEY *pub_key, const uint8_t *buf, size_t len);


#ifdef __cplusplus
}
#endif

#endif //__LSQUIC_CRYPTO_H__
