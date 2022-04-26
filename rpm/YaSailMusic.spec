Name:       YaSailMusic

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}
Summary:    The unofficial client for Yandex Music
Version:    0.1
Release:    1
Group:      Qt/Qt
License:    GNU GPLv3
URL:        https://vk.com/mobilevika
Source0:    %{name}-%{version}.tar.bz2
Requires:   sailfishsilica-qt5 >= 0.10.9
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  desktop-file-utils

%description
The unofficial client for Yandex Music by Ilya Vysotsky

%prep
%setup -q -n %{name}-%{version}

%build

%qtc_qmake5
%qtc_make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%qmake5_install

desktop-file-install --delete-original       \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

mkdir -p %{buildroot}/%{_datadir}/icons/hicolor/108x108/apps/
cp %{buildroot}/%{_datadir}/YaSailMusic/qml/img/%{name}.png %{buildroot}/%{_datadir}/icons/hicolor/108x108/apps/
mkdir -p %{buildroot}/%{_datadir}/icons/hicolor/128x128/apps/
cp %{buildroot}/%{_datadir}/YaSailMusic/qml/img/%{name}.png %{buildroot}/%{_datadir}/icons/hicolor/128x128/apps/
mkdir -p %{buildroot}/%{_datadir}/icons/hicolor/172x172/apps/
cp %{buildroot}/%{_datadir}/YaSailMusic/qml/img/%{name}.png %{buildroot}/%{_datadir}/icons/hicolor/172x172/apps/

%files
%defattr(-,root,root,-)
%{_bindir}
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop

%{_datadir}/icons/hicolor/*/apps/%{name}.png
