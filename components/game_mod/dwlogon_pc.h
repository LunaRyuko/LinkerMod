#pragma once

struct bdAuthService
{
	char _pad1[0x150];
	char m_steamCookieKey[88];
};
STATIC_ASSERT_OFFSET(bdAuthService, m_steamCookieKey, 0x150);

static bdAuthService*& g_authService = *(bdAuthService **)0x11169A8;

void dwLogonSeAcquiredSteamTicket();