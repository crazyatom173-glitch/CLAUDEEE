# MSVC Build Tools Yükseltme - Plan

Çözüm: C:\Users\crazy\OneDrive\Masaüstü\Yeni klasör (11)\CS2_Internal.sln
İlgili proje: C:\Users\crazy\OneDrive\Masaüstü\Yeni klasör (11)\CS2_Internal.vcxproj

## Amaç
Platform Toolset 'v180' eksikliğinden kaynaklanan MSB8020 hatasını düzeltmek ve çözümü yeniden başarılı şekilde derleyebilir hale getirmek.

## İlgili Bulgular
- assessment.md'de raporlanan tek hata: MSB8020 - v180 araçları eksik. (Dosya: .github/upgrades/CppBuildToolsUpgrade/assessment.md)

## Düzeltme Seçenekleri (tavsiye edilenler)
1) Yerel makineye v180 MSVC Build Tools (Platform Toolset v180) yükleyin (Önerilen)
   - Avantaj: Proje ayarlarına dokunmadan, mevcut yapı hedeflerini koruyarak sorunu giderir.
   - Dezavantaj: Kullanıcının yönetici izni ve Visual Studio Installer kullanımı gerektirir.
   - Adımlar (özet): Visual Studio Installer -> Modify -> Individual components -> "MSVC v180 - VS 2026 C++ x64/x86 build tools" veya benzeri bileşeni seçip yükleyin. Yükleme tamamlandıktan sonra tam yeniden derleme yapılacak.

2) Projeyi mevcut yüklü araç setine yeniden hedefleyin (ör. v143 veya Visual Studio'nun önerdiği en güncel platform toolset)
   - Avantaj: Yeni makinelere taşımada ekstra yük gerektirmez; tek seferlik proje güncellemesi.
   - Dezavantaj: Platform toolset değişikliği bazı kod/bağımlılık uyumsuzluklarına yol açabilir; ek kaynak değişiklikleri gerekebilir.
   - Adımlar (özet):
	 a) Proje dosyasını unload etmek
	 b) .vcxproj içinde <PlatformToolset>v180</PlatformToolset> satırını <PlatformToolset>v143</PlatformToolset> (veya uygun sürüm) ile değiştirmek
	 c) cppupgrade_validate_vcxproj_file ile doğrulamak
	 d) Projeyi reload etmek
	 e) Incremental build (cppupgrade_build_and_get_issues) ile derlemek ve hataları düzeltmek

## Önerilen Plan (adım adım)
- Plan, iki ana paralel yol sunar: (A) "Yükleme" veya (B) "Yeniden hedefleme". Öncelik: önce 1) Yükleme denensin; eğer kullanıcı yüklemeyi tercih etmiyorsa veya yükleme mümkün değilse 2) Yeniden hedefleme uygulanır.

Adımlar:
1. Hazırlık
   - Not: Değişiklik yapmadan önce hiçbir kod veya proje dosyası düzenlenmeyecek.
   - Dosyalar: Plan ve assessment dosyaları oluşturuldu: .github/upgrades/CppBuildToolsUpgrade/plan.md, assessment.md

2. Seçenek A: v180 yüklemesi (Öncelikli)
   - 2.1 Kullanıcıdan v180 yüklemesini yapmasını isteyin (ya da yükleme talimatlarını takip edin).
   - 2.2 Yükleme tamamlandıktan sonra tool 'cppupgrade_rebuild_and_get_issues' ile tam yeniden derleme yapılarak hata/uyarılar toplanacak.
   - 2.3 Eğer MSB8020 giderildiyse işlem tamam, final rebuild raporu oluşturulacak.

3. Seçenek B: Projeyi yeniden hedefleme (eğer A mümkün değilse veya kullanıcı isterse)
   - 3.1 Projeyi unload etme (upgrade_unload_project aracı kullanılacak) — NOT: Bu adım Execution aşamasında yapılacak.
   - 3.2 .vcxproj dosyasındaki <PlatformToolset> değerini v180 -> v143 (veya uygun olan) olarak değiştir.
   - 3.3 cppupgrade_validate_vcxproj_file ile doğrula.
   - 3.4 Projeyi yeniden yükle (reload_project).
   - 3.5 Incremental build (cppupgrade_build_and_get_issues) çalıştır, ortaya çıkan hataları sırayla düzelt.
   - 3.6 Her düzeltmeden sonra incremental build tekrarlanacak; son adımda cppupgrade_rebuild_and_get_issues ile tam yeniden derleme yapılacak.

## Görevler (Tasks)
- task-1: "v180 yüklemesi" — Kullanıcının veya bizim tarafından Visual Studio Installer üzerinden v180 araçlarının yüklenmesi, ardından tam rebuild ile doğrulama.
- task-2: "Proje yeniden hedefleme: CS2_Internal.vcxproj" — .vcxproj düzenlemesi, doğrulama, reload ve derleme doğrulama.

Her görev tamamlandığında 'progress-details.md' oluşturulacak ve build sonuçları eklenecektir.

## Riskler ve Notlar
- Yeniden hedefleme (v180 -> v143) bazı API/ikili uyumsuzluklar veya üçüncü taraf bağımlılık sorunlarına yol açabilir; bu durumda ek kod değişiklikleri gerekebilir.
- .vcxproj düzenlemesi yapmadan önce proje unload edilmelidir; düzenleme sonrası cppupgrade_validate_vcxproj_file ile doğrulanmadan projeyi reload etmeyin.
- Değişiklikleri mevcut branch'e doğrudan commit etmeyin; yeni bir çalışma dalı (branch) oluşturulması önerilir. Commit/branch işlemleri için kullanıcının iznini isteyeceğim.

## Sonraki Adım
Plan hazır. Onay verirseniz Execution (Uygulama) aşamasına geçip task-1 ile başlayacağım (ilk olarak v180 yüklemesini doğrulamaya çalışacağım). Eğer farklı tercih ediyorsanız lütfen belirtin.

---
