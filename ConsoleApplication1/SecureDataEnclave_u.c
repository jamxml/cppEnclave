#include "SecureDataEnclave_u.h"
#include <errno.h>

typedef struct ms_getPD_t {
	char* ms_outbuffer;
	size_t ms_len;
	int ms_index;
} ms_getPD_t;

typedef struct ms_getArraySize_t {
	int ms_retval;
} ms_getArraySize_t;

typedef struct ms_sgx_oc_cpuidex_t {
	int* ms_cpuinfo;
	int ms_leaf;
	int ms_subleaf;
} ms_sgx_oc_cpuidex_t;

typedef struct ms_sgx_thread_wait_untrusted_event_ocall_t {
	int ms_retval;
	const void* ms_self;
} ms_sgx_thread_wait_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_set_untrusted_event_ocall_t {
	int ms_retval;
	const void* ms_waiter;
} ms_sgx_thread_set_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_setwait_untrusted_events_ocall_t {
	int ms_retval;
	const void* ms_waiter;
	const void* ms_self;
} ms_sgx_thread_setwait_untrusted_events_ocall_t;

typedef struct ms_sgx_thread_set_multiple_untrusted_events_ocall_t {
	int ms_retval;
	const void** ms_waiters;
	size_t ms_total;
} ms_sgx_thread_set_multiple_untrusted_events_ocall_t;

static sgx_status_t SGX_CDECL SecureDataEnclave_sgx_oc_cpuidex(void* pms)
{
	ms_sgx_oc_cpuidex_t* ms = SGX_CAST(ms_sgx_oc_cpuidex_t*, pms);
	sgx_oc_cpuidex(ms->ms_cpuinfo, ms->ms_leaf, ms->ms_subleaf);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL SecureDataEnclave_sgx_thread_wait_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_wait_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_wait_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_wait_untrusted_event_ocall(ms->ms_self);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL SecureDataEnclave_sgx_thread_set_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_set_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_untrusted_event_ocall(ms->ms_waiter);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL SecureDataEnclave_sgx_thread_setwait_untrusted_events_ocall(void* pms)
{
	ms_sgx_thread_setwait_untrusted_events_ocall_t* ms = SGX_CAST(ms_sgx_thread_setwait_untrusted_events_ocall_t*, pms);
	ms->ms_retval = sgx_thread_setwait_untrusted_events_ocall(ms->ms_waiter, ms->ms_self);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL SecureDataEnclave_sgx_thread_set_multiple_untrusted_events_ocall(void* pms)
{
	ms_sgx_thread_set_multiple_untrusted_events_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_multiple_untrusted_events_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_multiple_untrusted_events_ocall(ms->ms_waiters, ms->ms_total);

	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * func_addr[5];
} ocall_table_SecureDataEnclave = {
	5,
	{
		(void*)(uintptr_t)SecureDataEnclave_sgx_oc_cpuidex,
		(void*)(uintptr_t)SecureDataEnclave_sgx_thread_wait_untrusted_event_ocall,
		(void*)(uintptr_t)SecureDataEnclave_sgx_thread_set_untrusted_event_ocall,
		(void*)(uintptr_t)SecureDataEnclave_sgx_thread_setwait_untrusted_events_ocall,
		(void*)(uintptr_t)SecureDataEnclave_sgx_thread_set_multiple_untrusted_events_ocall,
	}
};

sgx_status_t getPD(sgx_enclave_id_t eid, char* outbuffer, size_t len, int index)
{
	sgx_status_t status;
	ms_getPD_t ms;
	ms.ms_outbuffer = outbuffer;
	ms.ms_len = len;
	ms.ms_index = index;
	status = sgx_ecall(eid, 0, &ocall_table_SecureDataEnclave, &ms);
	return status;
}

sgx_status_t getArraySize(sgx_enclave_id_t eid, int* retval)
{
	sgx_status_t status;
	ms_getArraySize_t ms;
	status = sgx_ecall(eid, 1, &ocall_table_SecureDataEnclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

