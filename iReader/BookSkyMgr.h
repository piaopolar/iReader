#pragma once
#include "../BaseCode/Singleton.h"

#include <map>
#include <string>
#include <vector>

class CBookSkyMgr : public Singleton<CBookSkyMgr>
{
public:
	struct CHAPTER_INFO
	{
		CHAPTER_INFO();
		int m_nNo;
		std::string m_strName;
		std::string m_strUrl;
		std::string m_strLastUpdate;
		std::string m_strExtra;

		bool m_bTextInit;
		std::string m_strText;

		void InitTextInfo(void);
		std::string GetTextDumpStr(void);
	};

	struct SITE_INFO
	{
		SITE_INFO();
		std::string m_strSiteName;
		std::string m_strSiteUrl;

		bool m_bChapterInfoInit;
		std::vector<CHAPTER_INFO> m_vecChapterInfo;

		void InitChapterInfo(void);
		std::string GetChapterDumpStr(void);

		std::string GetTextDumpStr(int nChapterIndex);
	};

	struct BOOK_INFO
	{
		BOOK_INFO();

		// basic info
		std::string m_strBookName;
		std::string m_strBookUrl;
		std::string m_strPicUrl;
		std::string m_strAuthor;
		std::string m_strAuthorUrl;
		std::string m_strDesc;
		std::string m_strLastUpdate;
		std::string m_strLastUpdateUrl;
		std::string GetBasicInfoDumpStr (void)const;

		// site info
		bool m_bSiteInfoInit;
		std::vector<SITE_INFO> m_vecSiteInfo;

		void InitSiteInfo(void);
		std::string GetSiteInfoDumpStr (void);

		// chapter info
		void InitChapterInfo(int nSite);
		std::string GetChapterDumpStr (int nSite);

		// text info
		std::string GetTextDumpStr(int nSite, int nChapter);
	};

	struct BOOK_SET_INFO
	{
		std::vector<CBookSkyMgr::BOOK_INFO> m_vecBookInfo;
		std::string m_strNextPage;
	};

	struct SEARCH_INFO
	{
		std::string m_strKeyWord;
		int m_nType;
		int m_nPage;
	};

	enum { KEYWORD_TYPE_BOOKNAME, KEYWORD_TYPE_AUTHOR, };
private:
	std::map<std::string, SEARCH_INFO> m_mapUrl2SearchInfo;
public:
	CBookSkyMgr(void);
	~ CBookSkyMgr(void);
public:
	CBookSkyMgr::BOOK_SET_INFO GetBookInfo(const char *pszKeyWord, int nType);
	CBookSkyMgr::BOOK_SET_INFO GetBookInfo(const char *pszSearchUrl);

	std::vector<CBookSkyMgr::SITE_INFO> GetSiteInfo(const char *pszBookUrl);

	bool GetUrlSearchInfo(const char *pszUrl, OUT CBookSkyMgr::SEARCH_INFO &rInfo) const;
};
